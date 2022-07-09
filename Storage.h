//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_STORAGE_H
#define BITVECTOR_STORAGE_H

#include <string>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <map>
using namespace std;

namespace fs = std::filesystem;

template<typename uint_t>
class Storage {
private:
    std::string fileName;
    map<string, string> metaData;
    int D;
    int pages;
    std::fstream file;

public:
    int getD() const;

    Storage(const std::string fileName, int D);

    bool open();

    bool create();

    virtual void appendPage(const uint_t buffer[]);

    virtual void updatePage(const uint_t buffer[], uint_t p);

    virtual void readPage(uint_t *&buffer, uint_t p);

    int getPages(){
        return pages;
    }
    //D-1 word of bits + 1 D for rank
    uint_t bitsPerPage(){
        return  (D-1)*sizeof(uint_t);
    }
    uint_t pageSize(){
        return D*sizeof(uint_t);
    }

    uint_t size(){
        return pageSize()*pages;
    }

    void setMeta(string key, string value){
        metaData[key]=value;
    }

    string getMeta(string key){
        return metaData[key];
    }

    ~Storage();
private:
    void readMetaData();
    void saveMetaData();
};

template<typename w>
Storage<w>::~Storage() {
    file.close();
    saveMetaData();
}

template<typename uint_t>
void Storage<uint_t>::appendPage(const uint_t buffer[]) {
    assert (file.is_open());
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<const char*> (buffer), pageSize());
    pages++;
}

template<typename uint_t>
void Storage<uint_t>::updatePage(const uint_t buffer[], uint_t p) {
    assert (p < pages);
    assert(file.is_open());
    file.seekp(p * pageSize());
    file.write(reinterpret_cast<const char*> (buffer), pageSize());
    pages++;
}


template<typename uint_t>
void Storage<uint_t>::readPage(uint_t *&buffer, uint_t p) {
    assert (p < pages);
    assert(file.is_open());
    file.seekg(p * pageSize());
    file.read(reinterpret_cast<char*> (buffer), pageSize());
}

template<typename uint_t>
bool Storage<uint_t>::open() {
    if(!file.is_open()) {
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        readMetaData();
    }
    return file.is_open();
}

template<typename uint_t>
Storage<uint_t>::Storage(const std::string fileName, int D):fileName(fileName), D(D) {
    pages=0;
    metaData["d"]=to_string(D);
}

template<typename uint_t>
bool Storage<uint_t>::create() {
    if(!(fs::exists(fileName)) ){
        std::ofstream createFile(fileName);
        assert(createFile.is_open()); // cambiar por excepción
        createFile.close();
        saveMetaData();
    }
    return fs::exists(fileName);
}

template<typename uint_t>
void Storage<uint_t>::readMetaData() {
    string name=fileName;
    name+="_METADATA";
    ifstream metafile(name.data());
    string key, value;
    while(metafile>>key>>value){
        metaData[key]=value;
    }
    metafile.close();
}

template<typename uint_t>
void Storage<uint_t>::saveMetaData() {
    string name=fileName;
    name+="_METADATA";
    ofstream metafile(name.data());
    string key, value;
    for(auto item:metaData){
        metafile<<item.first<<" "<<item.second<<endl;
    }
    metafile.close();
}

template<typename uint_t>
int Storage<uint_t>::getD() const {
    return D;
}

#endif //BITVECTOR_STORAGE_H
