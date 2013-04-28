#include "Precompile.h"
#include "FSEditLog.h"

using namespace std;


FSEditLog::FSEditLog(FSImage* fsimage){

    _isSyncRunning = false;
    _fsImage = fsimage;
    _version = _fsImage->getVersion();

    open(OUT);
}


FSEditLog::~FSEditLog(){
}


void FSEditLog::open(OpenMode mode) {
    string file = Config::get("dfs.editLog.path");

    try{

        if(mode == IN) {
            _editIStream = make_shared<ifstream>(file);
        } else {
            _editOStream = make_shared<ofstream>(file);
            _editOStream->write(reinterpret_cast<const char*>(&_version),
                                 sizeof(_version));
        }
    } catch (exception& e){
        Log::write(ERROR, e.what());
        exit(EXIT_FAILURE);
    } catch (char* e) {
        Log::write(ERROR, e);
        throw(e);
    }
}


/*
flush.
*/
void FSEditLog::logSync() {
    std::unique_lock<std::mutex> ulock(_m_log);

    _isSyncRunning = true;

    _editOStream->flush();

    _isSyncRunning = false;

}


void FSEditLog::logEdit(stringstream* ss) {
    std::unique_lock<std::mutex> ulock(_m_log);

    if(_editOStream->good()) {
        _editOStream->write(ss->str().c_str(), ss->str().size());
    }
}


//close edit log output stream
void FSEditLog::close(){
    if(_editOStream->is_open()) {
        Log::write(INFO, "closing edits ...");

        _editOStream->close();
    } else {
        Log::write(DEBUG, "edits is NOT open. do nothing ...");
    }
}


/*
load edit log from disk.
*/
void FSEditLog::loadEdits() {

    int numEdits = 0;
    float logVersion = 0;

    string clientName;
    string clientMachine;

    int numOpAdd = 0,
        numOpClose = 0,
        numOpDelete = 0;
//        numOpRename = 0, numOpSetRepl = 0, numOpMkDir = 0,
//        numOpSetPerm = 0, numOpSetOwner = 0, numOpSetGenStamp = 0,
//        numOpTimes = 0, numOpGetDelegationToken = 0,
//        numOpRenewDelegationToken = 0, numOpCancelDelegationToken = 0,
//        numOpUpdateMasterKey = 0, numOpOther = 0;

    long startTime = time(NULL);

    open(IN);

    bool avail = true;
    try{
        _editIStream->read((char*)&logVersion, sizeof(logVersion));
    }catch(...){
        avail = false;
        Log::write(ERROR,"fail to get logVersion.");
    }

    assert(avail);

    while(true){
        short opcode = -1;

        try{
            _editIStream->read(reinterpret_cast<char*>(&opcode),
                                sizeof(opcode));

            if(opcode==OP_INVALID) {
                Log::write(ERROR,"get OP_INVALID from editLog. break ...");
                break;
            }
        }catch(...){
            Log::write(ERROR,"get error when reading opcode. break ...");
            break;
        }

        numEdits++;

        switch(opcode) {
            case OP_ADD:
            case OP_CLOSE: {
                INodeFile* newNode;

                if(opcode==OP_ADD) {
                    newNode = new INodeFileUnderConstruction();

                    numOpAdd++;
                }
                else{
                    newNode = new INodeFile();

                    numOpClose++;
                }

                newNode->readFields(_editIStream.get());

                shared_ptr<Block> blk;
                for(int i =0; i< newNode->getBlockNum(); i++){
                    blk = make_shared<Block>();
                    blk->readFields(_editIStream.get());
                    newNode->addBlock(blk);
                }

                shared_ptr<Permission> perm = make_shared<Permission>();
                perm->readFields(_editIStream.get());
                newNode->setPermission(perm);

                if(opcode==OP_ADD){
                    clientName = Writable::readString(_editIStream.get());
                    clientMachine = Writable::readString(_editIStream.get());

                    ((INodeFileUnderConstruction*)newNode)->setClientName(clientName);
                    ((INodeFileUnderConstruction*)newNode)->setClientMachine(clientMachine);
                }

                //add to namespace
                shared_ptr<INode> pNode(newNode);
                _fsImage->addFile(pNode,false,true);

                Log::write(INFO, "loadEdits: ADD/CLOSE : %d - %s",
                           opcode, pNode->toString());

                break;
            }
            case OP_DELETE: {
                numOpDelete++;

                string path = Writable::readString(_editIStream.get());

                long timestamp = -1;
                _editIStream->read(reinterpret_cast<char*>(&timestamp),
                                sizeof(timestamp));

                int delCount = _fssys->deleteFile(path, timestamp);

                Log::write(INFO, "loadEdits: Deleted %s - file count : %d",
                            path, delCount);

                break;
            }
            default:
                Log::write(ERROR, "Invalid opcode %d.\n", opcode);
        }


        try{

        }catch(exception& e){
            if(_editIStream->is_open())
                _editIStream->close();

            Log::write(ERROR, e.what());
        }

    }

    if(_editIStream->is_open())
        _editIStream->close();

    long elapse = time(NULL) - startTime;
    Log::write(INFO, "FSEditLog::loadEdits takes %d secs\n", elapse);
}


/*log OPs*/

void FSEditLog::logOpenFile(string path, INodeFileUnderConstruction& newNode) {
    stringstream ss;

    //op code
    short op_add = OP_ADD;
    ss.write(reinterpret_cast<const char*>(&op_add),
              sizeof(op_add));

    // INode
    newNode.write(&ss);

    //blks
    vector<shared_ptr<Block>> blks = newNode.getBlocks();
    for(vector<shared_ptr<Block>>::iterator iter = blks.begin(); iter != blks.end(); iter++){
        (*iter)->write(&ss);
    }

    //write perm
    newNode.getPermission()->write(&ss);

    //write client name /machine
    Writable::writeString(&ss,newNode.getClientName());
    Writable::writeString(&ss,newNode.getClientMachine());

    //locs
    //version 1.0.4 doesn't write locs to edit log.

    logEdit(&ss);
}


void FSEditLog::logCloseFile(string path, INodeFile& newNode) {
    stringstream ss;

    //op code
    short op_close = OP_CLOSE;
    ss.write((char*)&op_close, sizeof(op_close));

    // INode
    newNode.write(&ss);

    //blks
    vector<shared_ptr<Block>> blks = newNode.getBlocks();
    for(vector<shared_ptr<Block>>::iterator iter = blks.begin(); iter != blks.end(); iter++){
        (*iter)->write(&ss);
    }

    //write perm
    newNode.getPermission()->write(&ss);

    //locs
    //version 1.0.4 doesn't write locs to edit log.

    logEdit(&ss);
}


void logDelete(string src, long timestamp) {
    stringstream ss;

    //op code
    short op_del = OP_DELETE;
    ss.write(reinterpret_cast<const char*>(&op_mkdir),
              sizeof(op_del));

    //path, modify time, access time
    Writable::writeString(&ss, src);

    ss.write(reinterpret_cast<const char*>(&timestamp),
             sizeof(timestamp));

    logEdit(&ss);
}


void FSEditLog::logMkDir(string path, INode& newNode) {
    stringstream ss;

    //op code
    short op_mkdir = OP_MKDIR;
    ss.write((char*)&op_mkdir, sizeof(op_mkdir));

    //path, modify time, access time
    Writable::writeString(&ss, newNode.getPath());

    long modTime= newNode.getModTime();
    ss.write((char*)&modTime,sizeof(modTime));

    long accessTime = newNode.getAccessTime();
    ss.write((char*)&accessTime,sizeof(accessTime));

    //write perm
    newNode.getPermission()->write(&ss);

    logEdit(&ss);
}

















