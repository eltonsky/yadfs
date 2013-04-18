#include "Precompile.h"
#include "FSImage.h"

using namespace std;


FSImage::FSImage()
{
    _imgVersion = Config::getInt("dfs.imgFile.version");
    _namespaceId = -1;
    _numFiles = 1; // _root always exists
    _genStamp = -1;
}



FSImage::FSImage(string imgFile) : FSImage()
{
    _imageFile = imgFile;
}


FSImage::~FSImage()
{
}


/* load image file to in mem struct
*/
void FSImage::loadImage() {
    ifstream imgStream;
    INodeDirectory* parent;
    INode* child;

    Log::write(INFO, "FSImage::loadImage : start loading...\n");

    try{
        imgStream.open(_imageFile.c_str(), ios::binary | ios::ate);
        long fileSize = imgStream.tellg();

        if(fileSize > 0){
            imgStream.seekg(0, ios::beg);

            // global param
            imgStream.read(reinterpret_cast<char*>(&_imgVersion),
                            sizeof(_imgVersion));

            imgStream.read(reinterpret_cast<char*>(&_namespaceId),
                            sizeof(_namespaceId));

            imgStream.read(reinterpret_cast<char*>(&_numFiles),
                            sizeof(_numFiles));

            imgStream.read(reinterpret_cast<char*>(&_genStamp),
                            sizeof(_genStamp));

            Log::write(DEBUG, "<img version:%f> <ns id:%d> <num files:%d> <gen stamp:%ld>\n",
                        _imgVersion, _namespaceId, _numFiles, _genStamp);

            if (imgStream.good()) {

                shared_ptr<INode> current;

                // load INodes
                for(int i=0; i < _numFiles; i++) {
                    current = make_shared<INode>();

                    current->readFields(&imgStream);
                    parent = INodeDirectory::getParent(current->getPath(), _root.get());

                    // if dir; can not use is_directory() here,
                    // that's not stored in image
                    if (current->getBlockNum() == 0){

                        if (parent==NULL) {
                            _root = make_shared<INodeDirectory>(current.get());
                            child = _root.get();
                        } else {
                            shared_ptr<INode> pNode =
                                make_shared<INodeDirectory>(current->getPath());

                            child = parent->addChild(pNode, true).get();
                        }

                    } else {
                        // if file
                        shared_ptr<INode> pNode =
                                make_shared<INodeFile>(current->getPath(),
                                                       current->getReplication(),
                                                       current->getBlockSize(),
                                                       current->getBlockNum());

                         shared_ptr<Block> blk;
                        // read blocks
                        for(int fileIndex = 0; fileIndex < current->getBlockNum(); fileIndex++) {
                            blk = make_shared<Block>();
                            blk->readFields(&imgStream);
                            (dynamic_cast<INodeFile*>(pNode.get()))->addBlock(blk);
                        }

                        child = parent->addChild(pNode, true).get();
                    }

                    if (child == NULL) {
                        Log::write(ERROR, "failed to load INode %s\n",
                                   current->getPath().c_str());
                        return;
                    }

                    //get permission for this INode
                    shared_ptr<Permission> perm =
                        make_shared<Permission>();

                    perm->readFields(&imgStream);

                    child->setPermission(perm);
                }

                // load data nodes


                // load file under construct

            }
        } else {
            // load default namespace structure in mem
            // root ('/') in namespace
            _root = make_shared<INodeDirectory>("/");
            _root->setPermission(make_shared<Permission>());

        }

    }catch(exception& exp){
        Log::write(ERROR, "FSImage::loadImage() : %s\n",
                   exp.what());

        if(imgStream.is_open())
            imgStream.close();

        return;
    }

    if(imgStream.is_open())
        imgStream.close();

    //mark image as ready
    _m_ready.lock();

    _ready = true;

    _m_ready.unlock();

    _m_cond_ready.notify_all();
}


// save in memory namespace to disk.
void FSImage::saveImage() {
    ofstream imgOStream;

    try{
        imgOStream.open(_imageFile.c_str(),ios::out | ios::binary);

        if(imgOStream.is_open()){
            imgOStream.write(reinterpret_cast<const char*>(&_imgVersion),
                              sizeof(_imgVersion));

            imgOStream.write(reinterpret_cast<const char*>(&_namespaceId),
                              sizeof(_namespaceId));

            imgOStream.write(reinterpret_cast<const char*>(&_numFiles),
                              sizeof(_numFiles));

            imgOStream.write(reinterpret_cast<const char*>(&_genStamp),
                              sizeof(_genStamp));

            //recursively write each inode.
            saveINode(static_cast<INode*>(_root.get()), &imgOStream);
            cout<<"_root count " << _root.use_count()<<endl;
            saveINodeWrap(_root.get(), &imgOStream);
        }


    } catch(exception& exp) {
        Log::write(ERROR, "FSImage::saveImage : %s\n",
                   exp.what());

        if(imgOStream.is_open())
            imgOStream.close();

        return;
    }

    if(imgOStream.is_open())
        imgOStream.close();

}


void FSImage::saveINode(INode* currNode, ofstream* ofs) {

///
cout<<currNode->getPath()<<" : "<<currNode->isDirectory() << endl;

    string path = currNode->getPath();
    Writable::writeString(ofs, path);

    short rep = currNode->getReplication();
    ofs->write(reinterpret_cast<const char*>(&rep),
               sizeof(rep));

    long modTime = currNode->getModTime();
    ofs->write(reinterpret_cast<const char*>(&modTime),
               sizeof(modTime));

    long accessTime = currNode->getAccessTime();
    ofs->write(reinterpret_cast<const char*>(&accessTime),
                sizeof(accessTime));

    long blkSize = currNode->getBlockSize();
    ofs->write(reinterpret_cast<const char*>(&blkSize),
                sizeof(blkSize));

    int blkNum = currNode->getBlockNum();
    ofs->write(reinterpret_cast<const char*>(&blkNum),
                sizeof(blkNum));

    if(currNode->isDirectory()) {
        long nsQuota = currNode->getNSQuota();

        ofs->write(reinterpret_cast<const char*>(&nsQuota),
                    sizeof(nsQuota));

        long dsQuota = currNode->getDSQuota();

        ofs->write(reinterpret_cast<const char*>(&dsQuota),
                    sizeof(dsQuota));
    } else {
        //write blocks
        vector<shared_ptr<Block>> blocks =
            (dynamic_cast<INodeFile*>(currNode))->getBlocks();

        for(int i=0; i<currNode->getBlockNum();i++){
            blocks[i]->write(ofs);
        }
    }

    currNode->getPermission()->write(ofs);
}

/*first loop store all children to image file;
* 2nd loop store children recursively. This way keeps the order of
* INode data in image file.
*/
void FSImage::saveINodeWrap(INodeDirectory* currNode, ofstream* ofs){

    vector<shared_ptr<INode>> children = currNode->getChildren();

    vector<shared_ptr<INode>>::iterator iter;

    for(iter= children.begin(); iter != children.end(); iter++) {
        saveINode(iter->get(), ofs);
    }

    iter= children.begin();
    for(; iter != children.end(); iter++) {
        if(iter->get()->isDirectory()) {
            saveINodeWrap(dynamic_cast<INodeDirectory*>(iter->get()),
                           ofs);
        }
    }
}


/*wait until fsimage finished loading.*/
void FSImage::_waitForReady() {
    std::unique_lock<std::mutex> ulock(_m_ready);

    long start = time(NULL);

    while (!_ready) {
      try {

        _m_cond_ready.wait_for(ulock, chrono::milliseconds(100),
                [this] { return _ready; });

        Log::write(INFO, "wait for %d for image ready...\n",
                   time(NULL)-start);
      } catch (exception& e) {
          Log::write(ERROR, "FSImage::_waitForReady() : %s\n",e.what());
      }
    }
}


void FSImage::addFile(shared_ptr<INode> sNode, bool protect, bool inheritPerm) {
    INodeDirectory* parent = NULL;

    if(protect)
        _waitForReady();

    try{
        parent = INodeDirectory::getParent(sNode->getPath(), _root.get());

        sNode->setParent(parent);

        parent->addChild(sNode, inheritPerm);

        _numFiles++;

        Log::write(INFO, "added file " + sNode.get()->getPath());
    } catch(exception& e) {
        Log::write(ERROR, "fail to add file %s : %s\n",
                   sNode->getPath().c_str(), e.what());
    }

}





