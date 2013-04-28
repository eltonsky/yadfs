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
#include "Server.h"
#include "FSNameSystem.h"

class NameNode : public DataNodeProtocol, public ClientProtocol {

public:

    static NameNode& getInstance()
    {
        static NameNode instance;

        return instance;
    }
    ~NameNode();

    void loadNameSpace();
    void saveNameSpace();
    void start();
    void close();

    inline IPC::Server* getRpcServer() {return &_rpcServer;}
    inline FSNameSystem& getNameSystem() {return _fssystem;}


    /// This is invoked by DFSClient.create from clientprotocol
    shared_ptr<Writable> create(shared_ptr<StringWritable> path,
                shared_ptr<Permission> perm,
                shared_ptr<StringWritable> clientName,
                shared_ptr<StringWritable> clientMachine,
                shared_ptr<NumWritable<bool>> overwrite,
                shared_ptr<NumWritable<bool>> createParent,
                shared_ptr<NumWritable<short>> replication,
                shared_ptr<NumWritable<long>> blockSize);

    private:

        IPC::Server _rpcServer;
        FSNameSystem& _fssystem = FSNameSystem::getFSNamesystem();

        /// TODO: create metric class
        //static NameNodeInstrumentation metrics;

        NameNode();
        NameNode(NameNode const&);
        void operator=(NameNode const&);


};

#endif // NAMENODE_H
