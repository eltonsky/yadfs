#ifndef FSNAMESYSTEM_H
#define FSNAMESYSTEM_H
#include "FSImage.h"
#include "FSEditLog.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>

class FSNameSystem
{
    public:

        static FSNameSystem& getFSNamesystem() {
            static FSNameSystem fssys;

            return fssys;
        }
        virtual ~FSNameSystem();

        void saveNameSpace();
        void loadNameSpace();
        void initialize();

        inline shared_ptr<FSImage> getFSImage() {return _fsImage;}
        inline shared_ptr<FSEditLog> getEditLog() {return _fsEditLog;}
        inline INodeDirectory* getRoot() {return _fsImage->getRoot().get();}

    private:

        shared_ptr<FSImage> _fsImage;
        shared_ptr<FSEditLog> _fsEditLog;

        FSNameSystem();
        FSNameSystem(FSNameSystem const&);
        void operator=(FSNameSystem const&);
};

#endif // FSNAMESYSTEM_H
