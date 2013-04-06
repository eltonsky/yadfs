#include "Call.h"

namespace Server {
    const int recheck_interval = 100;

    ////Call
    Call::Call()
    {
        //ctor
    }

    Call::Call(shared_ptr<Connection> conn) : _connection(conn) {
    }


    bool Call::read() {
        if(_connection.get() == NULL)
            return false;

        tcp::socket* sock = _connection->getSock().get();

        try{
            size_t l = boost::asio::read(*(sock),
                boost::asio::buffer(&_call_id, sizeof(_call_id)));
             if(l <= 0) {
                Log::write(ERROR, "Fail to read call id\n");
                return -1;
            }

Log::write(DEBUG, "#### 1\n");

            _class = Writable::readString(sock);

Log::write(DEBUG, "#### 2\n");

            _method = Writable::readString(sock);

Log::write(DEBUG, "#### 3\n");

            //params
            size_t size = -1;
            l = boost::asio::read(*(sock),
                boost::asio::buffer(&size, sizeof(size)));

            if(l <= 0) {
                Log::write(ERROR, "Fail to read size of params, class %s, method %s\n",
                       _class.c_str(), _method.c_str());
                return -1;
            }


Log::write(DEBUG, "#### 4, _params.size %d\n", size);

            _params.reserve(size);

            for(size_t i =0; i < size; i++) {

Log::write(DEBUG, "#### 5, i %d\n", i);

                string param_class = Writable::readString(sock);

                _params.push_back(Method::getNewInstance(param_class));

                _params[i]->readFields(sock);
            }

        } catch(exception& e) {
            Log::write(ERROR, "Exception when read a call <%s> : %s\n",
                       toString().c_str(), e.what());
            return false;
        }

        return true;
    }


    ///@decprecated : use connection::processResponse instead.
    bool Call::write() {

//        try{
//            tcp::socket* sock = _connection->getSock().get();
//
//            boost::asio::write(*sock, boost::asio::buffer((const char*)&_call_id, sizeof(_call_id)));
//
//            Writable::writeString(sock, _strVal);
//        }
//         catch(exception& e) {
//            Log::write(ERROR, "Exception when write a call result <%s : %s> : %s \n",
//                       toString().c_str(), _strVal.c_str(), e.what());
//            return false;
//        }

        return true;
    }


    string Call::toString() {
        stringstream ss;
        ss <<"call_id "<<_call_id<<", class "<<_class<<" , method "<<_method;
        return ss.str();
    }


    string Call::print() {
        stringstream ss;
        ss <<"call_id "<<_call_id<<", class "<<_class<<" , method "<<_method;

        for(size_t i=0; i < _params.size(); i++) {
            ss<<"\n param "<<i<<" : " << _params[i]->toString();
        }

        return ss.str();
    }


    Call::~Call()
    {
        //dtor
    }

}








