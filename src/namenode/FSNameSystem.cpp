#include "Precompile.h"
#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{
    _fsImage = make_shared<FSImage>(Config::get("pfs,imgFile,path"));
    _fsEditLog = make_shared<FSEditLog>(_fsImage.get());
}

FSNameSystem::~FSNameSystem()
{
}


void FSNameSystem::initialize(){
}


void FSNameSystem::saveNameSpace(){

    _fsImage->saveImage();
}


void FSNameSystem::loadNameSpace(){

    _fsImage->loadImage();

    _fsImage->print(getRoot());
}









