#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cassert>
#include "Utility.h"
std::string Utility::readFile(const char* path){
#ifndef NDEBUG
    std::cout<<"Getting file: "<<path<<"\n";
#endif
    // Open the file, but freak out if not valid.
    std::ifstream file=std::ifstream(path);
    assert(file.good());
    if(!file.good())
    {
        throw std::runtime_error((std::string)"Couldn't open file for loading: "+path);
    }

    // Read the file contents into a char buffer.
    std::stringstream buffer;buffer << file.rdbuf();
    return buffer.str();
}