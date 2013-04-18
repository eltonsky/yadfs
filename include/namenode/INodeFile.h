#ifndef INODEFILE_H
#define INODEFILE_H
#include "INode.h"
#include "Block.h"
#include <vector>

class INodeFile : public INode
{
    public:
        INodeFile();
        INodeFile(string, short, long, int);
        INodeFile(INodeFile*);
        INodeFile(INodeFile& INodeFile);
        INodeFile& operator = (INodeFile&);
        virtual ~INodeFile();

        virtual void readFields(istream* is);
        virtual void write(ostream* os) {INode::write(os);}
        bool isDirectory() {return false;}

        inline vector<shared_ptr<Block>>& getBlocks() {return std::ref(_blocks);}

        void addBlock(shared_ptr<Block>);
        void setBlock(int, shared_ptr<Block>);
        void spaceConsumed();
        void getLastBlock();
        void print(bool);

    protected:

        vector<shared_ptr<Block>> _blocks;
    private:
};

#endif // INODEFILE_H
