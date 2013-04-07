#include "DFSClient.h"

DFSClient::DFSClient()
{
    // init log
    Log::init("DFSClient");
}


///TODO:pick up server ep from config
DFSClient::DFSClient(string server, int port) : _namenode(server, port)
{
    DFSClient();
}


DFSClient::~DFSClient()
{
    //dtor
}


int DFSClient::create(string path, int replication, shared_ptr<Permission> perm) {

    try{
        shared_ptr<StringWritable> path_ = make_shared<StringWritable>(path);
        shared_ptr<IntWritable> rep_ = make_shared<IntWritable>(replication);

        shared_ptr<Writable> res = _namenode.create(path_, rep_, perm);

        return dynamic_pointer_cast<IntWritable>(res)->get();

    }catch(exception &e){
        Log::write(ERROR, "Failed to create file in name space - path %s, rep %d, perm %s -- exp: %s\n",
                   path.c_str(), replication, perm->toString().c_str(), e.what());

        return -1;
    }
}
