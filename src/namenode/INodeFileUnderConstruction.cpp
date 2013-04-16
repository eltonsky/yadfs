#include "Precompile.h"
#include "INodeFileUnderConstruction.h"

INodeFileUnderConstruction::INodeFileUnderConstruction()
{
    //ctor
}

INodeFileUnderConstruction::INodeFileUnderConstruction(string path,short rep,
        long modTime,long preferBlkSize,int numBlks,int numLocs)
{
    _path = path;
    _replication = rep;
    _modTime = modTime;
    _preferredBlockSize = preferBlkSize;
    _numBlocks = numBlks;
    _numLocations = numLocs;
}


void INodeFileUnderConstruction::write(ostream* os) {
    Writable::writeString(os, _path);

    os->write(reinterpret_cast<const char*>(&_replication),
              sizeof(_replication));

    os->write(reinterpret_cast<const char*>(&_modTime),
              sizeof(_modTime));

    os->write(reinterpret_cast<const char*>(&_accessTime),
               sizeof(_accessTime));

    os->write(reinterpret_cast<const char*>(&_preferredBlockSize),
               sizeof(_preferredBlockSize));

    os->write(reinterpret_cast<const char*>(&_numBlocks),
               sizeof(_numBlocks));
}


void INodeFileUnderConstruction::readFields(istream* is) {

    _path = Writable::readString(is);

    is->read(reinterpret_cast<char*>(&_replication),
              sizeof(_replication));

    is->read(reinterpret_cast<char*>(&_modTime),
              sizeof(_modTime));

    is->read(reinterpret_cast<char*>(&_accessTime),
              sizeof(_accessTime));

    is->read(reinterpret_cast<char*>(&_blockSize),
              sizeof(_blockSize));

    is->read(reinterpret_cast<char*>(&_numBlocks),
              sizeof(_numBlocks));
}


INodeFileUnderConstruction::~INodeFileUnderConstruction()
{
    //dtor
}




