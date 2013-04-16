#include "Precompile.h"
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
#include "Config.h"
#include "Log.h"
#include "MethodWritable.h"
#include "NameNode.h"
#include "Test1.h"
#include "Test2.h"

IPC::Server* server_ptr;
atomic<bool> teminated(false);

void terminate(int signum) {
    if(signum == SIGINT && teminated == false) {
        teminated.store(true);

        Log::write(INFO, " Terminating .. \n");

        long start = Utils::getTime();

        server_ptr->stop();

        server_ptr = NULL;

        Log::write(INFO, "Terminate takes %ld miliseconds\n", (Utils::getTime() - start));

        exit(0);
    } else {
        Log::write(INFO, "SIGNAL %d ignored ..\n", signum);
    }
}


int main(int argc, char** argv)
{
    bool ifServer = true;

    ///load config
    Config::load();

    if (argc < 2)
    {
        cout<<"Is this client or server? <type> required."<<endl;
        return -1;

    } else if(strcmp(argv[1],"-c") == 0) {

        ifServer = false;

    } else if (strcmp(argv[1], "-s") ==0) {

    } else {
        cout<< "Unrecognized type '" << argv[1]<<"'"<<endl;
        return -1;
    }

    try {

        if(!ifServer) {

            Test2 t2;
            t2.test21(Config::get("dfs.namenode.ip"),
                      Config::getInt("dfs.namenode.port"),
                      4, 50);

        } else {

            signal(SIGINT, terminate);

            server_ptr = NameNode::getInstance().getRpcServer();

            NameNode::getInstance().start();
        }

    }  catch(exception& e) {
        Log::write(ERROR, e.what());
        return -1;
    }

    Log::write(INFO, "Finish !\n");
    return 0;
}











