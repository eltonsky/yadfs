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
        virtual ~INodeDirectory();

        inline vector<shared_ptr<INode>>& getChildren() {
            return _children;
        }
        virtual inline bool isDirectory() {return true;}

        shared_ptr<INode> addChild(shared_ptr<INode>, bool);

        static INodeDirectory* getParent(string path,
                                            INodeDirectory* root);
        INode* findChild(string path);

        void print(bool);

    protected:

        vector<shared_ptr<INode>> _children;

    private:
};

#endif // INODEDIRECTORY_H



