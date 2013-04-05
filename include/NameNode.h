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

class NameNode : DataNodeProtocol{
    public:

    NameNode();
    ~NameNode();

    virtual shared_ptr<Writable> create(shared_ptr<StringWritable> Arg0,
                                        shared_ptr<IntWritable> Arg1) {
        int res = 12345;

        return make_shared<IntWritable>(res);
    }

    //...

};

#endif // NAMENODE_H
