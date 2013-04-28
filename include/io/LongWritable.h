#ifndef LongWritable_H
#define LongWritable_H

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

class LongWritable : public Writable
{
    public:
        LongWritable();
        LongWritable(const LongWritable&);
        LongWritable(long v);
        ~LongWritable();

        int readFields(tcp::socket * sock);
        int write(tcp::socket * sock, int start=0);
        string toString();
        string printToString();
        string getClass();
        int length();

        inline long get() {return _value;}

    protected:
    private:
        long _value;
};

#endif // LongWritable_H
