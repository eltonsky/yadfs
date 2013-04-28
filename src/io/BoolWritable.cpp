#include "Precompile.h"
#include "BoolWritable.h"

BoolWritable::BoolWritable(){}

BoolWritable::BoolWritable(bool v) : _value(v)
{
    //ctor
}


// assume an int is read once and for all.
int BoolWritable::readFields(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    int l = boost::asio::read(*sock,
            boost::asio::buffer(&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "BoolWritable::readFields: expected length %d, read length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


// assume an int is written once and for all.
int BoolWritable::write(tcp::socket * sock, int start){

    if(sock == NULL)
        return -1;

    int l = boost::asio::write(*sock, boost::asio::buffer((const char*)&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "BoolWritable::write: expected length %d, write length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


string BoolWritable::printToString() {
    return std::to_string(_value);
}


string BoolWritable::toString() {
    return std::to_string(_value);
}


string BoolWritable::getClass(){
    return "BoolWritable";
}


int BoolWritable::length() {
    return sizeof(_value);
}


BoolWritable::~BoolWritable()
{
    //dtor
}
