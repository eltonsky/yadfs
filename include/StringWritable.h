#ifndef STRINGWRITABLE_H
#define STRINGWRITABLE_H


#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "Writable.h"

using namespace std;
using boost::asio::ip::tcp;

class StringWritable : public Writable
{
    public:
        StringWritable();
        StringWritable(string);
        virtual ~StringWritable();

        virtual int readFields(tcp::socket * sock);
        virtual int write(tcp::socket * sock, int start=0);
        virtual string toString();
        virtual string printToString();

        virtual inline string getClass() {return "StringWritable";}

        inline string get() {return _value;}

        virtual int length();

    protected:
    private:

        string _value;
};

#endif // STRINGWRITABLE_H
