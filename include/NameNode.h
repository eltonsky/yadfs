#ifndef NAMENODE_H
#define NAMENODE_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "DataNodeProtocol.h"
#include "ClientProtocol.h"
#include "ipc/Server.h"

class NameNode : public DataNodeProtocol, public ClientProtocol {

public:

    static NameNode& getInstance()
    {
        static NameNode instance;

        return instance;
    }

    NameNode();
    ~NameNode();

    void start();

    void close();


    inline IPC::Server* getRpcServer() {return &_rpcServer;}


    virtual shared_ptr<Writable> create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm);

    /// TODO: add more functions

    private:

        IPC::Server _rpcServer;



        NameNode(NameNode const&);
        void operator=(NameNode const&);


};

#endif // NAMENODE_H
