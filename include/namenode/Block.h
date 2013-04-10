#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <fstream>
#include "Writable.h"

using namespace std;

class Block : public Writable
{
    public:
        Block();
        Block(long id, long len, long genStamp);
        virtual ~Block();

        inline const long getBlkID() {return _blockID;}
        inline const long getBlkLength() {return _blockLength;}
        inline const long getBlkGenStamp() {return _genStamp;}

        virtual void readFields(istream*);
        virtual void write(ostream*);
        virtual string toString();

        void print();

    protected:
        long _blockID;
        long _blockLength;
        long _genStamp;
    private:
};

#endif // BLOCK_H
