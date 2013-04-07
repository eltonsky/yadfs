#include<iostream>
#include "tinyxml2.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Log.h"

using namespace std;
using namespace tinyxml2;

#ifndef CONFIG_H
#define CONFIG_H

class Config
{
    public:
        Config();
        virtual ~Config();

        static void load();
        static void load(vector<string> files);
        static void load(string file);

        /*
        * the format of name : "file.leve1.level2....levelN"
        */
        static int getInt(string name);
        static string get(string name);

        ///@deprecated
        static const char* get(string name, char* attr);

        static void print(map<string,string>& conf);

    private:
        static string _getElement(string names);

        //static map<string,shared_ptr<XMLDocument>> _docs;
        static map<string,string> _confs;
        static map<string,string> _confFiles;

        static void loadNextElement(XMLElement* elmt,
                                    string parentName,
                                    bool isBase);

        const static string configFile;

};

#endif // CONFIG_H
