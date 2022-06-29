//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_PAGES_H
#define BITVECTOR_PAGES_H

#include <filesystem>
#include <string>
namespace fs = std::filesystem;

int createFolders(std::string name){
    std::string vectorDir = "../bitvectors/" + name + "/vectorPages";
    std::string selectDir = "../bitvectors/" + name + "/selectPages";
    fs::create_directories(vectorDir);
    fs::create_directory(selectDir);
    return 0;
}

#endif //BITVECTOR_PAGES_H
