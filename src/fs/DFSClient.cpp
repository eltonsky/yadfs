#include "Precompile.h"
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


int DFSClient::create(string path,
                      shared_ptr<Permission> perm,
                      string clientName,
                      string clientMachine,
                      bool overwrite,
                      bool createParent,
                      short replication,
                      long blockSize) {

    try{
        shared_ptr<StringWritable> path_ = make_shared<StringWritable>(path);
        shared_ptr<StringWritable> clientName_ = make_shared<StringWritable>(clientName);
        shared_ptr<StringWritable> clientMachine_ = make_shared<StringWritable>(clientMachine);
        shared_ptr<NumWritable<bool>> overwrite_ = make_shared<NumWritable<bool>>(overwrite);
        shared_ptr<NumWritable<bool>> createParent_ = make_shared<NumWritable<bool>>(createParent);
        shared_ptr<NumWritable<short>> rep_ = make_shared<NumWritable<short>>(replication);
        shared_ptr<NumWritable<long>> blockSize_ = make_shared<NumWritable<long>>(blockSize);

        shared_ptr<Writable> res = _namenode.create(path_, perm,
                                                    clientName_, clientMachine_,
                                                    overwrite_, createParent_,
                                                    rep_, blockSize_);

        return dynamic_pointer_cast<IntWritable>(res)->get();

    }catch(exception &e){
        Log::write(ERROR, "Failed to create file in name space - path %s, rep %d, perm %s -- exp: %s\n",
                   path.c_str(), replication, perm->toString().c_str(), e.what());

        return -1;
    }
}











