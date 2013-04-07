#ifndef WRITABLE_H
#define WRITABLE_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_array.hpp>
#include "Log.h"

using boost::asio::ip::tcp;
using namespace std;

class Writable
{
    public:
        Writable();
        virtual ~Writable();
        virtual int readFields(tcp::socket * sock);
        virtual int write(tcp::socket * sock, int start=0);
        // write private values to strstream
        virtual string toString();

        // print private values for test
        virtual string printToString();
        virtual string getClass();
        virtual int length();

        static int writeString(tcp::socket*sock, string str);
        static string readString(tcp::socket * sock);

    protected:
    private:
};

#endif // WRITABLE_H
