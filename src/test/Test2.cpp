#include "Test2.h"

Test2::Test2()
{
    //ctor
}

Test2::~Test2()
{
    //dtor
}


void Test2::test201(string server_host, int port, int rep) {

    DataNodeProtocol namenode(server_host, port);

    for(int i = 0 ; i < rep; i++) {
        shared_ptr<IntWritable> a1 = make_shared<IntWritable>(12);
        shared_ptr<IntWritable> a2 = make_shared<IntWritable>(23);
        shared_ptr<IntWritable> a3 = make_shared<IntWritable>(34);
        shared_ptr<IntWritable> a4 = make_shared<IntWritable>(45);

        shared_ptr<StringWritable> a5 = make_shared<StringWritable>("abc.sh");

        shared_ptr<Writable> res = namenode.create(a5,a1);

        Log::write(INFO, "In thread %ld : %d #### CLIENT RESULT : %s\n" ,
                    (long int)syscall(SYS_gettid), i ,res->printToString().c_str());

    }
}


bool Test2::test20(string server_host, int port, int thread, int rep) {
    vector<boost::thread> ts(thread);

    for(int i =0; i < thread; i++) {
        ts[i] = boost::thread(boost::bind(&Test2::test201, this, server_host, port, rep));
    }

    for(int i =0; i < thread; i++) {
        ts[i].join();
    }

    return true;
}


void Test2::test211(string server_host, int port, int rep) {

    DFSClient dfsClient(server_host, port);

    for(int i = 0 ; i < rep; i++) {

        shared_ptr<Permission> perm = make_shared<Permission>("elton", "yadfsGroup", 777);

        int res = dfsClient.create("block_1234567", perm, "eltonsky", "ethost1", true, true, 3, 67108864);

        Log::write(INFO, "In thread %ld : %d #### CLIENT RESULT : %d\n" ,
                    (long int)syscall(SYS_gettid), i , res);

    }
}


bool Test2::test21(string server_host, int port, int thread, int rep) {
    vector<boost::thread> ts(thread);

    for(int i =0; i < thread; i++) {
        ts[i] = boost::thread(boost::bind(&Test2::test211, this, server_host, port, rep));
    }

    for(int i =0; i < thread; i++) {
        ts[i].join();
    }

    return true;
}




