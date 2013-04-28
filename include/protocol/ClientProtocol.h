#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "RPC.h"
#include "NumWritable.h"
#include "StringWritable.h"
#include "Permission.h"

class ClientProtocol {
    public:

        ClientProtocol();
        ClientProtocol(string server_host, int port);
        virtual ~ClientProtocol();

        virtual shared_ptr<Writable> create(shared_ptr<StringWritable> path,
                                            shared_ptr<Permission> perm,
                                            shared_ptr<StringWritable> clientName,
                                            shared_ptr<StringWritable> clientMachine,
                                            shared_ptr<NumWritable<bool>> overwrite,
                                            shared_ptr<NumWritable<bool>> createParent,
                                            shared_ptr<NumWritable<short>> replication,
                                            shared_ptr<NumWritable<long>> blockSize);

        //...

    private:
        shared_ptr<tcp::endpoint> _ep;
};

#endif // CLIENTPROTOCOL_H
