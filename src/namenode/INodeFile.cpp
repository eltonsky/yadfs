#include "Precompile.h"
#include "INodeFile.h"

INodeFile::INodeFile()
{
}


INodeFile::INodeFile(string path, short replication, long blkSize, int numBlocks)
    :INode(path, replication, blkSize, numBlocks){
}


INodeFile::INodeFile(INodeFile* file) : INode(file){
    _blocks.clear();
    vector<shared_ptr<Block>>::iterator iter;

    for(iter = file->getBlocks().begin(); iter!=file->getBlocks().end(); ++iter) {
        shared_ptr<Block> blk =
            make_shared<Block>((*iter)->getBlkID(),
                               (*iter)->getBlkLength(),
                               (*iter)->getBlkGenStamp());

        _blocks.push_back(blk);
    }
}


INodeFile::INodeFile(INodeFile& file) {
    _blocks.clear();
    vector<shared_ptr<Block>>::iterator iter;

    for(iter = file.getBlocks().begin(); iter!=file.getBlocks().end(); ++iter) {
        shared_ptr<Block> blk =
            make_shared<Block>((*iter)->getBlkID(),
                               (*iter)->getBlkLength(),
                               (*iter)->getBlkGenStamp());

        _blocks.push_back(blk);
    }
}


INodeFile& INodeFile::operator = (INodeFile& file) {

    _blocks.clear();
    vector<shared_ptr<Block>>::iterator iter;

    for(iter = file.getBlocks().begin(); iter!=file.getBlocks().end(); ++iter) {
        shared_ptr<Block> blk =
            make_shared<Block>((*iter)->getBlkID(),
                               (*iter)->getBlkLength(),
                               (*iter)->getBlkGenStamp());

        _blocks.push_back(blk);
    }

    return *this;
}


void INodeFile::addBlock(shared_ptr<Block> blk) {
    shared_ptr<Block> block = make_shared<Block>(blk->getBlkID(),
                                                 blk->getBlkLength(),
                                                 blk->getBlkGenStamp());
    _blocks.push_back(block);
}


void INodeFile::setBlock(int index, shared_ptr<Block> blk) {
    vector<shared_ptr<Block>>::iterator iter = _blocks.begin();

    shared_ptr<Block> block = make_shared<Block>(blk->getBlkID(),
                                                 blk->getBlkLength(),
                                                 blk->getBlkGenStamp());

    _blocks.insert(iter+index, block);
}


void INodeFile::readFields(istream* is) {
    INode::readFields(is);
}


// print out for test
void INodeFile::print(bool recursive) {
    cout<<"\n"+this->getPath()+":"<<endl;

    vector<shared_ptr<Block>>::iterator iter = _blocks.begin();

    for(;iter != _blocks.end();iter++){
        (*iter)->print();
    }

}


INodeFile::~INodeFile()
{
}






