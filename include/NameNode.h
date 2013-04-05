#ifndef NAMENODE_H
#define NAMENODE_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "ClientProtocol.h"


class NameNode : public ClientProtocol {

public:

    NameNode();
    ~NameNode();

    virtual shared_ptr<Writable> create(shared_ptr<StringWritable> path,
                                        shared_ptr<IntWritable> replication,
                                        shared_ptr<Permission> perm) {
        int res = 12345;

        Log::write(INFO, "Creating file <path:%s> <rep:%d>  <perm:%s>",
                   path->printToString().c_str(), replication->get(),
                   perm->printToString().c_str());

        return make_shared<IntWritable>(res);
    }

    //...

};

#endif // NAMENODE_H
