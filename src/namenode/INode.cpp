#include "INode.h"
using namespace std;

INode::INode()
{
}

//file
INode::INode(string path,
             short replication,
             long blockSize,
             int numBlocks):
             _path(path),
             _replication(replication),
             _blockSize(blockSize),
             _numBlocks(numBlocks)
{
    INode();
}

//dir
INode::INode(string path): INode(){
    _path = path;
}


INode::INode(INode* node) : INode() {
    _path = node->getPath();
    _modTime = node->getModTime();
    _accessTime = node->getAccessTime();
    _replication = node->getReplication();
    _blockSize = node->getBlockSize();
    _numBlocks = node->getBlockNum();
}


void INode::write(ostream* os) {
    Writable::writeString(os, _path);

    os->write(reinterpret_cast<char *>(&_replication),
                sizeof(_replication));

    os->write(reinterpret_cast<char *>(&_modTime),
               sizeof(_modTime));

    os->write(reinterpret_cast<char *>(&_accessTime),
               sizeof(_accessTime));

    os->write(reinterpret_cast<char *>(&_blockSize),
               sizeof(_blockSize));

    os->write(reinterpret_cast<char *>(&_numBlocks),
               sizeof(_numBlocks));
}


void INode::readFields(istream* is) {

    _path = Writable::readString(is);

    is->read(reinterpret_cast<char *>(&_replication),
              sizeof(_replication));

    is->read(reinterpret_cast<char *>(&_modTime),
             sizeof(_modTime));

    is->read(reinterpret_cast<char *>(&_accessTime),
             sizeof(_accessTime));

    is->read(reinterpret_cast<char *>(&_blockSize),
             sizeof(_blockSize));

    is->read(reinterpret_cast<char *>(&_numBlocks),
             sizeof(_numBlocks));

    if (_numBlocks == 0) {
        is->read((char*)&_nsQuota, sizeof(_nsQuota));

        is->read((char*)&_dsQuota, sizeof(_dsQuota));
    }
}


void INode::print(bool recursive){cout<<"get called"<<endl;}


bool INode::operator==(const INode& other) const {
    return (_path == other._path);
}


INode::~INode()
{
    //dtor
}







