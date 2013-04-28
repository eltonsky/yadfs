#ifndef ShortWritable_H
#define ShortWritable_H

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

class ShortWritable : public Writable
{
    public:
        ShortWritable();
        ShortWritable(const ShortWritable&);
        ShortWritable(short v);
        ~ShortWritable();

        int readFields(tcp::socket * sock);
        int write(tcp::socket * sock, int start=0);
        string toString();
        string printToString();
        string getClass();
        int length();

        inline short get() {return _value;}

    protected:
    private:
        short _value;
};

#endif // ShortWritable_H
