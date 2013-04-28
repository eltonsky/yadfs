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
                                    shared_ptr<Permission> perm,
                                    shared_ptr<StringWritable> clientName,
                                    shared_ptr<StringWritable> clientMachine,
                                    shared_ptr<NumWritable<bool>> overwrite,
                                    shared_ptr<NumWritable<bool>> createParent,
                                    shared_ptr<NumWritable<short>> replication,
                                    shared_ptr<NumWritable<long>> blockSize) {

    int res = _fssystem.startFile(path, perm, clientName, clientMachine,
                        overwrite, createParent, replication,
                        blockSize);

    /// TODO: increase metrics

    return make_shared<IntWritable>(res);
}














