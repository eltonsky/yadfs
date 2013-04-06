#include "StringWritable.h"


StringWritable::StringWritable()
{
    //ctor
}


StringWritable::StringWritable(string val) : _value(val) {
}


StringWritable::~StringWritable()
{
    //dtor
}


int StringWritable::write(tcp::socket * sock, int start_pos) {

    if(sock == NULL)
        return -1;

    size_t length = _value.length();

    // if start_pos is 0, assume length has not been passed
    if (start_pos == 0) {

        boost::asio::write(*sock, boost::asio::buffer((const char*)&length, sizeof(length)));
    }

    const char* start = _value.c_str() + start_pos;
    size_t left = length - start_pos;

    size_t wrote = sock->write_some(boost::asio::buffer(start, left));

    return wrote;
}


int StringWritable::readFields(tcp::socket * sock){

    if(sock == NULL)
        return -1;

    _value = Writable::readString(sock);

    return _value.length();
}


string StringWritable::printToString() {
    return _value;
}


string StringWritable::toString() {
    return _value;
}


unsigned char* StringWritable::toBytes() {return NULL;}

int StringWritable::length() {
    return _value.length();
}



