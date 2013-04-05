#include "IntWritable.h"

IntWritable::IntWritable(){}

IntWritable::IntWritable(int v) : _value(v)
{
    //ctor
}


/// NOTE: write & readFields need to be able to read bytes if this Writable
/// can be used as a value.
int IntWritable::readFields(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    size_t length;
    size_t l = boost::asio::read(*sock,
            boost::asio::buffer(&length, sizeof(length)));

    boost::scoped_array<char> buf(new char[length+1]);

    l = boost::asio::read(*sock,
            boost::asio::buffer(buf.get(), length));

    if(l != length) {
        Log::write(ERROR,
                   "IntWritable::readFields: expected length %d, read length %d\n",
                   length, l);
        return -1;
    }

    buf[length] = '\0';

    //convert
    _value = Utils::bitsToInt(buf.get());

    return l;
}


int IntWritable::write(tcp::socket * sock){

    if(sock == NULL)
        return -1;

    size_t length = sizeof(_value);

    boost::asio::write(*sock, boost::asio::buffer((const char*)&length, sizeof(length)));

    stringbuf sb;
    sb.sputn((const char*)&_value, sizeof _value);

    size_t l = boost::asio::write(*sock, boost::asio::buffer(sb.str().c_str(), length));

    if(l != length) {
        Log::write(ERROR,
                   "IntWritable::write: expected length %d, write length %d\n",
                   length, l);
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


IntWritable::~IntWritable()
{
    //dtor
}
