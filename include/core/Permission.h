#ifndef PERMISSION_H
#define PERMISSION_H

#include <iostream>
#include "Writable.h"

using namespace std;

class Permission : public Writable
{
    public:
        Permission();
        Permission(string,string,short);
        Permission(Permission*);
        Permission(Permission&);
        virtual ~Permission();

        virtual void readFields(istream*);
        virtual void write(ostream*);
        virtual int readFields(tcp::socket * sock);
        virtual int write(tcp::socket * sock, int start=0);
        virtual string toString();
        virtual string printToString();
        virtual string getClass();
        virtual int length();

        inline string getUserName() {return _userName;}
        inline string getGroupName() {return _groupName;}
        inline short getPerm() {return _perm;}

        friend std::ostream& operator<<(std::ostream&, const Permission&);

    protected:
        string _userName;
        string _groupName;
        short _perm;
    private:
};

#endif // PERMISSION_H
