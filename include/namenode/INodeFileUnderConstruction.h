#ifndef INODEFILEUNDERCONSTRUCTION_H
#define INODEFILEUNDERCONSTRUCTION_H

#include "Permission.h"
#include "DataNode.h"
#include "INodeFile.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

class INodeFileUnderConstruction : public INodeFile
{
    public:
        INodeFileUnderConstruction();
        INodeFileUnderConstruction(string,short,long,long,int,int);
        virtual ~INodeFileUnderConstruction();

        inline void setPreferredBlkSize(long prefBlkSize) {_preferredBlockSize = prefBlkSize;}
        inline const long getPreferredBlkSize() {return _preferredBlockSize;}
        inline string getClientName() {return _clientName;}
        inline string getClientMachine() {return _clientMachine;}
        inline void setClientName(string name) {_clientName = name;}
        inline void setClientMachine(string machine) {_clientMachine = machine;}

        virtual void write(ostream*);
        virtual void readFields(istream*);

        void setLocation(shared_ptr<DataNode> loc);

    protected:
        long _preferredBlockSize;
        string _clientName;
        string _clientMachine;
        int _numLocations;
        vector<shared_ptr<DataNode>> _locations;

    private:
};

#endif // INODEFILEUNDERCONSTRUCTION_H
