#ifndef FSNAMESYSTEM_H
#define FSNAMESYSTEM_H
#include "FSImage.h"
#include "FSEditLog.h"
#include "LeaseManager.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>

class FSNameSystem
{
    class SafeModeInfo;

    public:

        static FSNameSystem& getFSNamesystem() {
            static FSNameSystem fssys;

            return fssys;
        }

        ~FSNameSystem();

        void saveNameSpace();
        void loadNameSpace();
        void initialize();

        //TODO
        bool isInSafeMode();

        // call static func in INodeDirectory
        INode* findFileByPath(string path);

        // return parent object if found;
        INodeDirectory* verifyParent(string path);

        /// TODO: _fsEdit.logDel, del from _fsImage.del();
        // called for overwrite or delete INodeFile;
        // throw exp if path is a dir. remove inode and all
        // blocks.
        /// leave delete data blocks part
        int deleteFile(string path, long ts);
        int deleteFile(INode* node, INodeDirectory* parent, long ts);

        // remove lease; delete blocks from blocksMap; add blocks to
        // delete queue
        void removeBlocks(string path, vector<shared_ptr<Block>> delBlocks);

        /// TODO: check isInSafeMode,
        /// if exists and is a dir (can not overwrite)
        /// if !createParent, verifyParent
        /// if INode exists, try to recoverLease (TODO)
        /// if append, ensure file exists
        ///    else  if file exists, if overwrite delete it
        /// create INodeUnder.. add to fsImage.
        /// leaseManager.addLease
        int startFile(string src,
                      shared_ptr<Permission> perm,
                      string holder,
                      string clientMachine,
                      bool overwrite,
                      bool append,
                      bool createParent,
                      short replication,
                      long blockSize);

        inline shared_ptr<FSImage> getFSImage() {return _fsImage;}
        inline shared_ptr<FSEditLog> getEditLog() {return _fsEditLog;}
        inline INodeDirectory* getRoot() {return _fsImage->getRoot().get();}

    private:

        std::mutex _m_name_space;
        LeaseManager _leaseManager;
        SafeModeInfo* _safeMode;

        shared_ptr<FSImage> _fsImage;
        shared_ptr<FSEditLog> _fsEditLog;

        //TODO: map from Block to BlockInfo; create BlocksMap class.
        //map<shared_ptr<Block>,shared_ptr<BlockInfo>> _blocksMap;
        //map from datanode to block
        map<shared_ptr<DataNode>,shared_ptr<Block>> _dataNodesMap;

        FSNameSystem();
        FSNameSystem(FSNameSystem const&);
        void operator=(FSNameSystem const&);


        class SafeModeInfo{
        public:

            /// always return false for now.
            bool isOn();

        };

        class SafeModeMonitor {

        };
};

#endif // FSNAMESYSTEM_H
