#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include "Utility.h"
char * Utility::readFile(const char* path,char* out){
#ifndef NDEBUG
    std::cout<<"Getting file: "<<path<<"\n";
#endif
    // Open the file, but freak out if not valid.
    std::ifstream file(path);
    if(!file.good())
    {
        std::cerr << "Couldn't open file for loading: " << path << std::endl;

    }

    // Read the file contents into a char buffer.
    std::stringstream buffer;buffer << file.rdbuf();
    std::string fileContentsStr = buffer.str();
#ifndef NDEBUG
    std::cout<<fileContentsStr.c_str()<<"\n";
#endif
    out = new char[fileContentsStr.size()];
    strcpy(out,fileContentsStr.c_str());
    return out;
}