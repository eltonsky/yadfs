#include "Precompile.h"
#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{
    _fsImage =
        make_shared<FSImage>(Config::get("dfs.imgFile.path"));

    _fsImage->setFSNameSystem(this);

    _fsEditLog =
        shared_ptr<FSEditLog>(new FSEditLog(_fsImage.get()));

    _fsEditLog->setFSNameSystem(this);
}

FSNameSystem::~FSNameSystem()
{
}


void FSNameSystem::initialize(){
}


void FSNameSystem::saveNameSpace(){

    _fsImage->saveImage();
}


void FSNameSystem::loadNameSpace(){

    _fsImage->loadImage();

    _fsImage->print(getRoot());
}


bool FSNameSystem::isInSafeMode() {

    return _safeMode.isOn();
}


INode* FSNameSystem::findFileByPath(string path) {
    return INodeDirectory::findByPath(path, _fsImage->getRoot());
}


INodeDirectory* FSNameSystem::verifyParent(string path) {
    if(path.size() <= 1)
        return false;

    int i = path.find_last_of("/");
    string dir = path.substr(0,i);

    return findFileByPath(dir);
}


//call deleteFile(INode*, INodeDirectory*, long)
//
int FSNameSystem::deleteFile(string path, long ts) {

    INode* node = findFileByPath(path);

    INodeDirectory * p = verifyParent(path);

    deleteFile(node, p, ts);

    return 0;
}


//
int FSNameSystem::deleteFile(INode* node, INodeDirectory* parent, long ts) {
    unique_lock<std::mutex> ulock(_m_name_space);

    try{
        int delCount = _fsImage->deleteNode(node, parent, ts);

        if(delCount > 0) {
            _fsEditLog->logDelete(node->getPath(), ts);
        }

    } catch (exception& e) {
        Log::write(ERROR, "Failed to delete file node : %s, parent %s.",
                    node->toString(), parent->toString() );
        return -1;
    }

    return 0;
}


void FSNameSystem::removeBlocks(string path, vector<shared_ptr<Block>> delBlocks) {
    _leaseManager.removeLeaseFile(path);

    vector<shared_ptr<Block>>::iterator iter;
    for(iter=delBlocks.begin(); iter!=delBlocks.end();iter++) {
        ///TODO:
        //blockMap.erase
        //add block to to be delete list.
    }
}


/// check isInSafeMode,
/// if exists and is a dir (can not overwrite)
/// if !createParent, verifyParent
/// if INode exists, try to recoverLease (TODO)
/// if append, ensure file exists
///    else  if file exists, if overwrite delete it
/// create INodeUnder..
/// leaseManager.addLease
int FSNameSystem::startFile(string src,
              shared_ptr<Permission> perm,
              string holder,
              string clientMachine,
              bool overwrite,
              bool append,
              bool createParent,
              short replication,
              long blockSize) {

    if(isInSafeMode()) {
        Log::write(ERROR,
            "The file system is still in safe mode, please try again later.");
        return 0;
    }

    INode* node = findFileByPath(src);

    if(node != NULL && node->isDirectory()) {
        Log::write(ERROR,
            "Object %s exists and is a directory. Abort startFile!", src);

        return 0;
    }

    INodeDirectory* parent = NULL;
    if(!createParent) {
        INodeDirectory* parent = verifyParent(src);

        if(parent == NULL) {
            Log::write(ERROR,
            "'createParent' is not set, but parent doesn't"+
            " exist for %s. Abort startFile!", src);

            return 0;
        }
    }

    if(node !== NULL) {
        ///TODO: recover lease
    }

    ///TODO: get clientNode from datanode rep.
    shared_ptr<DataNodeDescriptor> clientNode;

    if(append) {

        if(node == NULL) {
            Log::write(ERROR,
                "Try to append, but file %s doesn't exist. Abort!", src);
            return 0;
        }

        /// TODO: move INodeFile to a INodeFileUnderConstruct,
        /// add lease, then write

    } else {
        if(overwrite && node != NULL) {
            if (parent == NULL)
                parent = verifyParent(src);

            int fileDeleted = deleteFile(node, parent, Utils::getTime());

            if(fileDeleted <= 0) {
                Log::write(ERROR,
                "Failed to delete file %s, can not overrite it. Abort!",
                src);

                return 0;
            }
        }

        // create new file obj
        shared_ptr<INode> file =
                make_shared<INodeFileUnderConstruction>(src,
                                                        perm,
                                                        replication,
                                                        blockSize,
                                                        Utils::getTime(),
                                                        holder,
                                                        clientMachine,
                                                        clientNode);
        _fsImage->addFile(file, true, false);

        _leaseManager.addLease(holder, src);

    }


    return 0;
}



/// SafeMode

/// TODO: always return true for now.
bool FSNameSystem::SafeModeInfo::isOn() {
    return true;
}












