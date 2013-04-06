#ifndef DFSCLIENT_H
#define DFSCLIENT_H

#include "IntWritable.h"
#include "StringWritable.h"
#include "ClientProtocol.h"
#include "Permission.h"

class DFSClient
{
    public:
        DFSClient();
        DFSClient(string serv, int port);
        ~DFSClient();

        int create(string path, int replication, shared_ptr<Permission> perm);

    protected:
    private:
        ClientProtocol _namenode;


};

#endif // DFSCLIENT_H
