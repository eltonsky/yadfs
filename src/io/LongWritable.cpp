#include "Precompile.h"
#include "LongWritable.h"

LongWritable::LongWritable(){}

LongWritable::LongWritable(long v) : _value(v)
{
    //ctor
}


// assume an int is read once and for all.
int LongWritable::readFields(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    size_t l = boost::asio::read(*sock,
            boost::asio::buffer(&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "LongWritable::readFields: expected length %d, read length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


// assume an int is written once and for all.
int LongWritable::write(tcp::socket * sock, int start){

    if(sock == NULL)
        return -1;

    int l = boost::asio::write(*sock, boost::asio::buffer((const char*)&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "LongWritable::write: expected length %d, write length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


string LongWritable::printToString() {
    return std::to_string(_value);
}


string LongWritable::toString() {
    return std::to_string(_value);
}


string LongWritable::getClass(){
    return "LongWritable";
}


int LongWritable::length() {
    return sizeof(_value);
}


LongWritable::~LongWritable()
{
    //dtor
}
