#include "Precompile.h"
#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{
    _fsImage =
        make_shared<FSImage>(Config::get("dfs.imgFile.path"));
    _fsEditLog =
        shared_ptr<FSEditLog>(new FSEditLog(_fsImage.get()));
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









