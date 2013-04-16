#include "Precompile.h"
#include "Writable.h"

Writable::Writable()
{
    //ctor
}

Writable::~Writable()
{
    //dtor
}

void Writable::write(ostream* os) {
}

void Writable::readFields(istream* is) {
}

int Writable::readFields(tcp::socket * sock) {return 0;}

int Writable::write(tcp::socket * sock, int start) {return 0;}

string Writable::toString() {return NULL;}

string Writable::printToString() {return NULL;}

int Writable::length() {return -1;}


void Writable::writeString(ostream* ofs, string str){
    int length = str.size()+1;

    ofs->write((char*)&length, sizeof(length));

    if (length > 1)
        ofs->write(str.c_str(), length);

}

string Writable::readString(istream* ifs) {
    int length = 0;

    ifs->read((char*)&length, sizeof(length));

    if(length > 1) {
        char buf[length];
        ifs->read(buf, length);

        return string(buf);
    }

    return string("");
}


int Writable::writeString(tcp::socket*sock, string str) {
    size_t length = str.size();

    if(sock == NULL || length == 0)
        return 0;

    boost::asio::write(*sock, boost::asio::buffer((const char*)&length, sizeof(length)));

    Log::write(DEBUG, "writeString : length is %d\n", length);

    size_t send_length =
        boost::asio::write(*sock, boost::asio::buffer(str.c_str(), str.size()));

    Log::write(DEBUG, "writeString : write %s, send length %d\n", str.c_str(), send_length);

    return send_length;
}


string Writable::readString(tcp::socket * sock){
    if(sock == NULL)
        return NULL;

    size_t length = -1;

    boost::asio::read(*sock,
            boost::asio::buffer(&length, sizeof(length)));

    Log::write(DEBUG, "readString : length is %d\n", length);

    if(length > 0) {

        boost::scoped_array<char> char_str(new char[length+1]);

        size_t reply_length = boost::asio::read(*sock,
            boost::asio::buffer(char_str.get(), length));

        // reply_length must be <= length
        if(reply_length > 0) {
            char_str[reply_length] = '\0';

            string str(char_str.get());

            Log::write(DEBUG, "readString : %s, reply length %d\n",
                       char_str.get(), reply_length);

            return str;
        }
    }

    return NULL;
}


string Writable::getClass() { return "Writable"; }




