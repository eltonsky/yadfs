#include "MethodWritable.h"

MethodWritable::MethodWritable()
{
    //ctor
}


MethodWritable::MethodWritable(string c, string m, vector<shared_ptr<Writable>> p)
    : _class_name(c), _method_name(m), _params(p) {}


int MethodWritable::readFields(tcp::socket * sock) {

    if(sock == NULL)
        return -1;

    try{

Log::write(DEBUG, "##- 1\n");

        //class
        _class_name = Writable::readString(sock);

Log::write(DEBUG, "##- 2\n");

        //method
        _method_name = Writable::readString(sock);



        //params
        size_t size = -1;
        size_t l = boost::asio::read(*sock,
            boost::asio::buffer(&size, sizeof(size)));

        if(l <= 0) {
            Log::write(ERROR, "Fail to read size of params, class %s, method %s\n",
                   _class_name.c_str(), _method_name.c_str());
            return -1;
        }

Log::write(DEBUG, "##- 3, _params.size %d\n", size);

        _params.reserve(size);
        for(size_t i =0; i < size; i++) {

Log::write(DEBUG, "##- 4, i %d\n", i);

            string param_class = Writable::readString(sock);

            _params.push_back(Method::getNewInstance(param_class));

            _params[i]->readFields(sock);
        }
    } catch(...){
        Log::write(ERROR, "Fail to write MethodWritable, class %s, method %s\n",
                   _class_name.c_str(), _method_name.c_str());
        return -1;
    }

    return 0;
}


int MethodWritable::write(tcp::socket * sock, int start){

    if(sock == NULL)
        return -1;

    try{

Log::write(DEBUG, "## 1\n");
        //class
        Writable::writeString(sock, _class_name);

Log::write(DEBUG, "## 2\n");

        //method
        Writable::writeString(sock, _method_name);
Log::write(DEBUG, "## 3\n");

        //params
        size_t size = _params.size();
        size_t l = boost::asio::write(*sock, boost::asio::buffer((const char*)&size, sizeof(size)));

        if(l <= 0) {
            Log::write(ERROR, "Fail to write size of params, class %s, method %s\n",
                   _class_name.c_str(), _method_name.c_str());
            return -1;
        }

        for(size_t i =0; i < size; i++) {
            Writable::writeString(sock, _params[i]->getClass());

Log::write(DEBUG, "## write clazz %s\n",_params[i]->getClass().c_str());

            _params[i]->write(sock);
        }

    } catch(...){
        Log::write(ERROR, "Fail to write MethodWritable, class %s, method %s\n",
                   _class_name.c_str(), _method_name.c_str());
        return -1;
    }

    return 0;
}


string MethodWritable::toString(){
    return NULL;
}


string MethodWritable::printToString() {
    string str;
    str.append("class : ");
    str.append(_class_name);
    str.append(" , method : ");
    str.append(_method_name);
    return str;
}


string MethodWritable::getClass() {
    return "MethodWritable";
}


unsigned char* MethodWritable::toBytes() {return NULL;}


int MethodWritable::length() {return -1;}


MethodWritable::~MethodWritable()
{
    //dtor
}



