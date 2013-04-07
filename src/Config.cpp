#include "Config.h"
using namespace tinyxml2;
using namespace std;


map<string,string> Config::_confs;
map<string,string> Config::_confFiles;
const string Config::configFile = "configFile";

Config::Config(){}

/*
load configs. start from base.xml and load all config files.
base.xml is hardcoded as the start point.
*/
void Config::load() {

    XMLDocument doc;

    if (doc.LoadFile("./conf/base.xml") != XML_SUCCESS){
       throw "Fail to load config file ./conf/base.xml";
    }

    // loop thru xml files
    // nav to base.log.conf, then go thru each entry
    loadNextElement(doc.FirstChildElement(),"",true);

    // load each conf file
    map<string,string>::iterator iter =
            _confFiles.begin();

    for(;iter!=_confFiles.end();iter++) {

        XMLDocument newDoc;

        if (newDoc.LoadFile(iter->second.c_str()) != XML_SUCCESS){
           throw "Fail to load config file " + iter->second;
        }

        loadNextElement(newDoc.FirstChildElement(), "",false);
    }


    //print out
//    print(_confFiles);
//    print(_confs);
}


void Config::loadNextElement(XMLElement* curr,
                             string parentName,
                             bool isBase) {

    if(curr != NULL) {
        const char* text = curr->GetText();

        string elmtName(parentName);

        if(parentName.length() > 0) {
            elmtName.append(".");
        }

        elmtName.append(curr->Name());

        if(text != NULL) {
            if(isBase &&
               parentName.find(configFile) != string::npos) {

                _confFiles.insert(pair<string,string>(elmtName,text));
            } else
                _confs.insert(pair<string,string>(elmtName,text));
        }

        loadNextElement(curr->FirstChildElement(), elmtName, isBase);

        loadNextElement(curr->NextSiblingElement(), parentName, isBase);
    }
}


int Config::getInt(string name) {
    string value = _getElement(name);

    return atoi(value.c_str());
}


string Config::get(string name) {
    return _getElement(name);
}


///@deprecated
const char* Config::get(string names, char* attr){
//    XMLElement* elmt = _getElement(names);
//
//    if(elmt==NULL){
//        Log::write(WARN, "can not find elmnt in " + names+ ". is there white space around it?");
//        return string().c_str();
//    }
//
//    return elmt->Attribute(attr);

return NULL;
}


/*
assume there's no duplicate conf name across all config files.
*/
string Config::_getElement(string name){

    map<string,string>::iterator iter =
            _confs.find(name);

    if(iter == _confs.end()) {
        return NULL;
    } else
        return iter->second;


    return NULL;
}


void Config::print(map<string,string>& conf) {
    //// print out
    map<string,string>::iterator iter1 =
                                    conf.begin();

    for(;iter1!=conf.end();iter1++) {
        cout<<iter1->first<<","<<iter1->second<<endl;
    }

}


Config::~Config()
{
    //dtor
}





