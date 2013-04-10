#include "Block.h"
using namespace std;


Block::Block()
{
    _blockID = 0;
    _blockLength = 0;
    _genStamp = -1;
}


Block::Block(long id, long len, long genStamp) :
                                    _blockID(id),
                                    _blockLength(len),
                                    _genStamp(genStamp)
{
}


Block::~Block()
{
    //dtor
}


void Block::readFields(istream* is) {
    is->read(reinterpret_cast<char *>(&_blockID),
             sizeof(_blockID));

    is->read(reinterpret_cast<char *>(&_blockLength),
             sizeof(_blockLength));

    is->read(reinterpret_cast<char *>(&_genStamp),
              sizeof(_genStamp));
}


void Block::write(ostream* os) {
    os->write(reinterpret_cast<const char *>(&_blockID),
               sizeof(_blockID));

    os->write(reinterpret_cast<const char *>(&_blockLength),
               sizeof(_blockLength));

    os->write(reinterpret_cast<const char *>(&_genStamp),
               sizeof(_genStamp));
}


string Block::toString() {
    stringstream ss;
    ss<<_blockID<<","<<_blockLength<<","<<_genStamp;
    return ss.str();
}


void Block::print(){
    cout<<_blockID<<","<<_blockLength<<","<<_genStamp<<endl;
}


