#ifndef DATANODEPROTOCOL_H
#define DATANODEPROTOCOL_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "RPC.h"
#include "StringWritable.h"

class DataNodeProtocol {
    public:

        DataNodeProtocol();
        virtual ~DataNodeProtocol();

        DataNodeProtocol(string server_host, int port){

            boost::system::error_code error;
            boost::asio::ip::address_v4 targetIP;
            targetIP = boost::asio::ip::address_v4::from_string(server_host.c_str(), error);

            _ep = make_shared<tcp::endpoint>();
            _ep->address(targetIP);
            _ep->port(port);
        }

        virtual shared_ptr<Writable> create(shared_ptr<StringWritable> Arg0,
                                            shared_ptr<IntWritable> Arg1) {

            return RPC::invoke("standard", _ep, "IntWritable", "NameNode", "create", Arg0, Arg1);
        }

        //...

    private:
        shared_ptr<tcp::endpoint> _ep;
};

#endif // DATANODEPROTOCOL_H
