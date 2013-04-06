#ifndef INTWRITABLE_H
#define INTWRITABLE_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "Writable.h"
#include "Utils.h"

using boost::asio::ip::tcp;
using namespace std;

class IntWritable : public Writable
{
    public:
        IntWritable();
        IntWritable(const Writable&);
        IntWritable(int v);
        virtual ~IntWritable();

        virtual int readFields(tcp::socket * sock);
        virtual int write(tcp::socket * sock, int start=0);
        virtual string toString();
        virtual string printToString();
        virtual string getClass();
        virtual int length();

        inline int get() {return _value;}

    protected:
    private:
        int _value;
};

#endif // INTWRITABLE_H
