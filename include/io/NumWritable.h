#ifndef NumWritable_H
#define NumWritable_H

#include "Precompile.h"
#include <typeinfo>
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

template <class T>
class NumWritable : public Writable
{
    public:
        NumWritable() {}

        NumWritable(const NumWritable<T>&);

        NumWritable(T v) : _value(v) {
        }

        ~NumWritable() {
        }

        int readFields(tcp::socket * sock) {
            if(sock == NULL)
                return -1;

            size_t l = boost::asio::read(*sock,
                    boost::asio::buffer(&_value, sizeof(_value)));

            if(l != sizeof(_value)) {
                Log::write(ERROR,
                           "NumWritable::readFields: expected length %d, read length %d\n",
                           sizeof(_value), l);
                return -1;
            }

            return l;
        }

        int write(tcp::socket * sock, int start=0) {
            if(sock == NULL)
                return -1;

            int l = boost::asio::write(*sock, boost::asio::buffer((const char*)&_value, sizeof(_value)));

            if(l != sizeof(_value)) {
                Log::write(ERROR,
                           "NumWritable::write: expected length %d, write length %d\n",
                           sizeof(_value), l);
                return -1;
            }

            return l;
        }

        string toString() {
            return std::to_string(_value);
        }

        string printToString() {
            return std::to_string(_value);
        }

        string getClass() {

            const char* valType = typeid(_value).name();

            if(valType == typeid(int).name()){
                return "NumWritable<int>";
            } else if(valType == typeid(short).name()){
                return "NumWritable<short>";
            } else if(valType == typeid(long).name()){
                return "NumWritable<long>";
            } else if(valType == typeid(bool).name()){
                return "NumWritable<bool>";
            }

            return NULL;
        }

        int length() {
            return sizeof(_value);
        }

        inline T get() {return _value;}

    protected:
    private:
        T _value;
};

#endif // NumWritable_H
