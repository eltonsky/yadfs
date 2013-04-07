#include "NameNode.h"


NameNode::NameNode() : _rpcServer(Config::getInt("dfs.namenode.port"))
{
    // init log
    Log::init("NameNode");
}


NameNode::~NameNode()
{
    //dtor
}


void NameNode::start() {
    _rpcServer.start();
}


shared_ptr<Writable> NameNode::create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm) {

    int res = 12345;

    return make_shared<IntWritable>(res);
}


