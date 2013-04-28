#ifndef LEASEMANAGER_H
#define LEASEMANAGER_H

#include <set>
#include <string>

using namespace std;

class LeaseManager
{
    class Lease;

    public:
        LeaseManager();
        ~LeaseManager();

        void addLease(string clientName, string src);
        /// TODO: remove src from lease, if no more srcs, remove lease
        void removeLease(shared_ptr<Lease> lease, string src);

        void renewLease(string clientName);

        shared_ptr<Lease> getLease(string clientName);

    protected:
    private:

        set<shared_ptr<Lease>> _leases;
        map<string,shared_ptr<Lease>> _fileLeases;
        std::mutex _m_lease;

        class Lease {


        private:
            string holder;
            long lastUpdated;
            set<string> _paths;
        };


        class Monitor {
            void checkLeases();
        };
};

#endif // LEASEMANAGER_H
