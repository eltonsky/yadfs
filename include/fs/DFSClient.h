#ifndef DFSCLIENT_H
#define DFSCLIENT_H

#include "NumWritable.h"
#include "IntWritable.h"
#include "StringWritable.h"
#include "ClientProtocol.h"
#include "Permission.h"
#include "Log.h"


class DFSClient
{
    public:
        DFSClient();
        DFSClient(string serv, int port);
        ~DFSClient();

        // start a DataOutputStream on success
        int create(string src,
                      shared_ptr<Permission> perm,
                      string holder,
                      string clientMachine,
                      bool overwrite,
                      bool createParent,
                      short replication,
                      long blockSize);

    protected:
    private:
        class LeaseChecker;
        class DataOutputStream;

        ClientProtocol _namenode;
        LeaseChecker* _leaseChecker;
        string _clientName;
        shared_ptr<DataOutputStream> _streamer;

        ///TODO: After new file is created in ns, add lease here.
        class LeaseChecker {

        public:
            // send renew
            void start();

            // add lease
            void put(string src, shared_ptr<DataOutputStream>);

            // when lease period(conf) is half over, namenode.renew(clientName)
            void renew();

            void close();

        private:

            map<string, shared_ptr<DataOutputStream>> _pendingCreates;

        };


        /// TODO:
        class DataOutputStream {

        };

};

#endif // DFSCLIENT_H
