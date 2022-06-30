//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_BITVECTOR_H
#define BITVECTOR_BITVECTOR_H

#include <string>
#include <string.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class Bitvector{
    private:
        std::string pathVector;
        std::string pathSelect;
        int pages;
        int pageSize;
    public:
        Bitvector(std::string name, int pageSize);
        int getNumberOfPages();
        int appendPage(char* buffer);
        int writePage(char* buffer, int p);
        int readPage(char* buffer, int p);
};

Bitvector::Bitvector(std::string name, int pageSize) {
    Bitvector::pathVector = "../vectors/" + name + "/vectorPages";
    Bitvector::pathSelect = "../vectors/" + name + "/selectPages";
    if(!fs::exists(pathVector))
        fs::create_directories(pathVector);
    if(!fs::exists(pathSelect))
        fs::create_directory(pathSelect);
    Bitvector::pages = 0;
    for (auto& p : fs::directory_iterator(pathVector))
        Bitvector::pages++;
    Bitvector::pageSize = pageSize;
}

int Bitvector::appendPage(char* buffer) {
    std::string pathPage = Bitvector::pathVector + "/" + std::to_string(Bitvector::pages) + ".bin";
    std::ofstream writePage(pathPage, std::ios::out | std::ios::binary);
    if(!writePage.is_open()){
        return -1;
    }
    writePage.write(buffer, Bitvector::pageSize);
    writePage.close();

    Bitvector::pages++;

    return 0;
}

int Bitvector::writePage(char *buffer, int p) {
    if( p >= Bitvector::pages ){
        return -1;
    }
    std::string pathPage = Bitvector::pathVector + "/" + std::to_string(p) + ".bin";
    std::ofstream writePage(pathPage, std::ios::out | std::ios::binary);
    if(!writePage.is_open()){
        return -1;
    }
    writePage.write(buffer, Bitvector::pageSize);
    writePage.close();
    return 0;
}

int Bitvector::readPage(char *buffer, int p) {
    if( p >= Bitvector::pages ){
        return -1;
    }
    std::string pathPage = Bitvector::pathVector + "/" + std::to_string(p) + ".bin";
    std::ifstream readPage(pathPage, std::ios::in | std::ios::binary);
    if(!readPage.is_open()){
        return -1;
    }
    readPage.readsome(buffer, Bitvector::pageSize);
    readPage.close();
    return 0;
}

int Bitvector::getNumberOfPages() { return Bitvector::pages; }

#endif //BITVECTOR_BITVECTOR_H
