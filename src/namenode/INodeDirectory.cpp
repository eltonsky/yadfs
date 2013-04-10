#include "INodeDirectory.h"
using namespace std;

INodeDirectory::INodeDirectory():INode(){}


INodeDirectory::INodeDirectory(string path) : INode(path)
{
}


INodeDirectory::INodeDirectory(INode* node) :
    INode(node)
{
}


INodeDirectory::INodeDirectory(INodeDirectory& dir) :
    INode(&dir)
{
    _children.clear();
    vector<shared_ptr<INode>>::iterator iter;
    for(iter = dir.getChildren().begin();iter != dir.getChildren().end();iter++){
        _children.push_back(*iter);
    }
}


INodeDirectory& INodeDirectory::operator = (INodeDirectory& dir) {

    _children.clear();
    vector<shared_ptr<INode>>::iterator iter;

    for(iter = dir.getChildren().begin();iter != dir.getChildren().end();iter++){
        _children.push_back(*iter);
    }

    return *this;
}


INodeDirectory::~INodeDirectory()
{
}


/*
    root path is indicated as "/". So if path.size() > 1,it's not root
*/
INodeDirectory* INodeDirectory::getParent(string path,
                                          INodeDirectory* root) {

///TODO: regex validate the path.

    // if the parent is not root
    if(path.size() > 1) {

        int i=0;
        string partialPath;
        INodeDirectory* currParent = root;

        while(i != -1) {
            i = path.find('/',i+1);
            if(i != -1){
                // parent of the dest node
                partialPath = path.substr(0,i);
                currParent = dynamic_cast<INodeDirectory*>(currParent->findChildByPath(partialPath));

                if(currParent == NULL) {
                    Log::write(ERROR, "partial path "+partialPath+" for file "+ path +" can not be located!");

                    return NULL;
                }

            }
        }

        return currParent;

    } else
        return NULL;

}

//
///*
//assume path.size() > 1
//always start from root so far.
//*/
//INodeDirectory* INodeDirectory::findByPath(string path, vector<short> pos, INodeDirectory* root){
//    vector<shared_ptr<INode>>::iterator iter;
//    INodeDirectory* p = root;
//    string partialPath;
//
//    for(size_t i=0; i< pos.size();i++){
//        partialPath = path.substr(0,pos[i]);
//
//        INode node(partialPath);
//
//        p = (INodeDirectory*) p->find_child(&node);
//
//        if (p == NULL) {
//            // part of the path doesn't exist
//            Log::write(ERROR, "partial path "+partialPath+" for file "+ path +" can not be located!");
//
//            return NULL;
//        }
//    }
//
//    return p;
//}


///*
//change string path to vector<string> paths.
//*/
//INode* INodeDirectory::addChild(INode* child, bool inheritPerm) {
//
//    if(inheritPerm){
//        Permission* perm = new Permission(this->getPermission());
//        child->setPermission(perm);
//    }
//
//    child->setModTime(time(NULL));
//
//    INode* exist = find_child(child);
//
//    if(exist == NULL){
//        INode* newNode;
//
//        if(child->isDirectory()) {
//            newNode = new INodeDirectory(child);
//        } else
//            newNode = new INodeFile((INodeFile*)child);
//
//        shared_ptr<INode> sChild(newNode);
//        _children.push_back(sChild);
//    }
//    else
//        return NULL;
//
//    return child;
//}


/*
change string path to vector<string> paths.
*/
shared_ptr<INode> INodeDirectory::addChild(shared_ptr<INode> sChild, bool inheritPerm) {

    if(inheritPerm){
        shared_ptr<Permission> perm = getPermission();
        sChild->setPermission(perm);
    }

    sChild->setModTime(time(NULL));

    INode* exist = findChildByPath(sChild->getPath());

    if(exist == NULL){
        _children.push_back(sChild);
        return sChild;
    }
    else
        return shared_ptr<INode>(exist);

}


INode* INodeDirectory::findChildByPath(string path) {
    vector<std::shared_ptr<INode>>::iterator iter;

    for(iter=_children.begin();iter!= _children.end();iter++){
        if(iter->get()->getPath() == path){
            return iter->get();
        }
    }

    return NULL;
}


// print out for test.
void INodeDirectory::print(bool recursive) {
    cout<<"\n"+this->getPath()+" : "<< this->getPermission()<<endl;

    vector<shared_ptr<INode>>::iterator iter;

    for(iter=_children.begin();iter!=_children.end();iter++){
        INode* node = iter->get();

        if(node->isDirectory())
            ((INodeDirectory*)node)->print(recursive);
        else
            ((INodeFile*)node)->print(recursive);
    }
}



