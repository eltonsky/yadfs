#ifndef BOOLWRITABLE_H
#define BOOLWRITABLE_H

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

class BoolWritable
{
    public:
        BoolWritable();
        BoolWritable(bool val);
        BoolWritable(const BoolWritable&);
        ~BoolWritable();

        int readFields(tcp::socket * sock);
        int write(tcp::socket * sock, int start=0);
        string toString();
        string printToString();
        string getClass();
        int length();

        inline bool get() {return _value;}

    protected:
    private:
        bool _value;
};

#endif // BOOLWRITABLE_H
