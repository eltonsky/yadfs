#ifndef INODEDIRECTORY_H
#define INODEDIRECTORY_H
#include "INode.h"
#include "INodeFile.h"
#include "Log.h"
#include <set>
#include <time.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

// it's a dir if numBlocks is 0
class INodeDirectory : public INode
{
    public:
        INodeDirectory();
        INodeDirectory(string);
        INodeDirectory(INode*);
        INodeDirectory(INodeDirectory&);
        INodeDirectory& operator = (INodeDirectory& );
        ~INodeDirectory();

        inline vector<shared_ptr<INode>>& getChildren() {
            return _children;
        }

        bool isDirectory() {return true;}

        shared_ptr<INode> addChild(shared_ptr<INode>, bool);

        INode* findChild(string path);

        int collectDeletingBlocks(vector<shared_ptr<Block>>&);

        void print(bool);

        static INodeDirectory* getParent(string path,
                                            INodeDirectory* root);
        static INode* findByPath(string path, INodeDirectory*);

    protected:

        vector<shared_ptr<INode>> _children;

    private:
};

#endif // INODEDIRECTORY_H



