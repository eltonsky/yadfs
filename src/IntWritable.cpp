#include "IntWritable.h"

IntWritable::IntWritable(){}

IntWritable::IntWritable(int v) : _value(v)
{
    //ctor
}


// assume an int is read once and for all.
int IntWritable::readFields(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    size_t l = boost::asio::read(*sock,
            boost::asio::buffer(&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "IntWritable::readFields: expected length %d, read length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


// assume an int is written once and for all.
int IntWritable::write(tcp::socket * sock, int start){

    if(sock == NULL)
        return -1;

    int l = boost::asio::write(*sock, boost::asio::buffer((const char*)&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "IntWritable::write: expected length %d, write length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


string IntWritable::printToString() {
    return std::to_string(_value);
}


string IntWritable::toString() {
    return std::to_string(_value);
}


unsigned char* IntWritable::toBytes() {
    return reinterpret_cast<unsigned char*>(&_value);
}


string IntWritable::getClass(){
    return "IntWritable";
}


int IntWritable::length() {
    return sizeof(_value);
}


IntWritable::~IntWritable()
{
    //dtor
}
