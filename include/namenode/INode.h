#ifndef INODE_H
#define INODE_H
#include "Block.h"
#include "Permission.h"
#include <iostream>
#include <string>
#include "Writable.h"

using namespace std;

class INode : public Writable
{
    public:
        INode();
        virtual ~INode();
        INode(string, short, long, int);
        INode(string);
        INode(INode*);
        bool operator==(const INode& other) const;

        inline const string getPath () const {return _path;};
        inline const int getBlockNum(){return _numBlocks;}
        inline const short getReplication() {return _replication;}
        inline const long getBlockSize() {return _blockSize;}
        inline const long getModTime() {return _modTime;}
        inline const long getAccessTime() {return _accessTime;};
        inline const shared_ptr<Permission> getPermission() {return _perm;}
        inline const INode* getParent() {return _parent;}
        inline const long getNSQuota() {return _nsQuota;}
        inline const long getDSQuota() {return _dsQuota;}
        inline void setPermission(shared_ptr<Permission> p) {_perm = p;}
        inline void setModTime(long modTime) {_modTime = modTime;}
        inline void setParent(INode* parent) {_parent = parent;}
        inline bool isDirectory() {return false;}

        virtual void readFields(istream*);
        virtual void write(ostream*);
        virtual void print(bool recursive);

    protected:

        long _modTime = 0;
        long _accessTime = 0;
        string _path;
        short _replication = 0;
        long _blockSize = 0;
        int _numBlocks = 0;
        long _nsQuota = 0; //max namespace allowed
        long _dsQuota = 0; //max diskspace allowed

        shared_ptr<Permission> _perm;
        INode* _parent = NULL; // must be a INodeDirectory

    private:


};


struct INodeCompare {
  bool operator() (INode* lhs, INode* rhs) const
  {return lhs->getPath().compare(rhs->getPath())<0;}
};


#endif // INODE_H
