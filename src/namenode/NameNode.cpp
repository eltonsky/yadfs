#include "Precompile.h"
#include "NameNode.h"


NameNode::NameNode() : _rpcServer(Config::getInt("dfs.namenode.port"))
{
    // init log
    Log::init("NameNode");

    _fssystem.initialize();
}


NameNode::~NameNode()
{
    //dtor
}


void NameNode::start() {
    _rpcServer.start();
}


/// Namespace management

void NameNode::loadNameSpace() {
    _fssystem.loadNameSpace();
}


void NameNode::saveNameSpace() {
    _fssystem.saveNameSpace();
}


/// RPC calls

shared_ptr<Writable> NameNode::create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm) {

    int res = 12345;

    return make_shared<IntWritable>(res);
}


