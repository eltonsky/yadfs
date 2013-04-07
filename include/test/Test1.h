#ifndef TEST1_H
#define TEST1_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include "BlockQueue.h"
#include <sys/syscall.h>
#include <boost/asio.hpp>
#include <chrono>
#include <condition_variable>
#include "unistd.h"
#include "stdlib.h"
#include "Listener.h"
#include <signal.h>
#include "Utils.h"
#include "Server.h"
#include "Client.h"
#include "Log.h"
#include "MethodWritable.h"
#include "RPC.h"
#include "Invocation.h"

class Test1
{
    public:
        Test1();
        virtual ~Test1();

        bool test1(std::string, int, int rep = 1);
        bool test2(std::string, int, int thread = 1, int rep =1);
        bool test10(string server_host, int port, int rep=1);
        bool test20(std::string, int, int thread = 1, int rep =1);

    protected:
    private:
};

#endif // TEST1_H
