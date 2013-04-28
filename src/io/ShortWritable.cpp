#include "Precompile.h"
#include "ShortWritable.h"

ShortWritable::ShortWritable(){}

ShortWritable::ShortWritable(short v) : _value(v)
{
    //ctor
}


// assume an int is read once and for all.
int ShortWritable::readFields(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    size_t l = boost::asio::read(*sock,
            boost::asio::buffer(&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "ShortWritable::readFields: expected length %d, read length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


// assume an int is written once and for all.
int ShortWritable::write(tcp::socket * sock, int start){

    if(sock == NULL)
        return -1;

    int l = boost::asio::write(*sock, boost::asio::buffer((const char*)&_value, sizeof(_value)));

    if(l != sizeof(_value)) {
        Log::write(ERROR,
                   "ShortWritable::write: expected length %d, write length %d\n",
                   sizeof(_value), l);
        return -1;
    }

    return l;
}


string ShortWritable::printToString() {
    return std::to_string(_value);
}


string ShortWritable::toString() {
    return std::to_string(_value);
}


string ShortWritable::getClass(){
    return "ShortWritable";
}


int ShortWritable::length() {
    return sizeof(_value);
}


ShortWritable::~ShortWritable()
{
    //dtor
}
