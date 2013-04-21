#ifndef FSIMAGE_H
#define FSIMAGE_H

#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include "Log.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <semaphore.h>
#include <time.h>
#include <mutex>
#include <condition_variable>
#include "DataNode.h"
#include "INodeFileUnderConstruction.h"
#include "INodeDirectory.h"
#include "INodeFile.h"

class FSImage
{
    public:
        FSImage();
        FSImage(string imgFile);
        ~FSImage();

        void loadImage();
        void saveImage();
        void saveINode(INode*, ofstream*);
        void saveINodeWrap(INodeDirectory*, ofstream*);
        void addFile(shared_ptr<INode>, bool protect, bool inheritPerm);
        void replaceRoot(shared_ptr<INode>);
        void setReady(bool state);
        bool getReady();


        inline shared_ptr<INodeDirectory> getRoot() { return _root; }

        inline void setVersion(float v) {_imgVersion = v;}

        inline float getVersion(){return _imgVersion;}

        inline void setFile(string file){_imageFile = file;}

        inline string getFile(){return _imageFile;}

        inline void setNamespaceID(int namespaceID){_namespaceId = namespaceID;}

        inline int getNamespaceID(){return _namespaceId;}

        inline void setNumFiles(int num){_numFiles = num;}

        inline int getNumFiles(){return _numFiles;}

        inline void setGenStamp(long gs) {_genStamp = gs;}

        inline long getGenStamp(){return _genStamp;}

        inline void print(INode* node) {node->print(true);}

    private:

        float _imgVersion;
        string _imageFile;
        int _namespaceId;
        int _numFiles;
        long _genStamp;
        shared_ptr<INodeDirectory> _root;
        vector<shared_ptr<DataNode>> dataNodes;
        vector<shared_ptr<INodeFileUnderConstruction>> fileUnderConst;
        bool _ready = false;

        std::mutex _m_ready;
        std::condition_variable _m_cond_ready;

        bool _waitForReady();
};

#endif // FSIMAGE_H





