//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_STORAGE_H
#define BITVECTOR_STORAGE_H

#include <string>
#include <fstream>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

template<typename w>
class Storage {
private:
    std::string fileName;

    int& pageSize;

    int pages;

    std::fstream file;

public:
    Storage(std::string fileName, int& pageSize) : pageSize(pageSize){

        Storage::fileName = fileName;
        pages = 0;
    }

    ~Storage();

    void appendPage(const w buffer[]);

    void writePage(const w buffer[], uint64_t p);

    void readPage(w buffer[], uint64_t p);

    bool open();
};

template<typename w>
Storage<w>::~Storage() {
    file.close();
}

template<typename w>
void Storage<w>::appendPage(const w buffer[]) {
    assert (file.is_open());
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<const char*> (buffer), pageSize);
    pages++;
}

template<typename w>
void Storage<w>::writePage(const w buffer[], uint64_t p) {
    assert (p < pages);
    assert(file.is_open());
    file.seekp(p * pageSize);
    file.write(reinterpret_cast<const char*> (buffer), pageSize);
    pages++;
}


template<typename w>
void Storage<w>::readPage(w buffer[], uint64_t p) {
    assert (p < pages);
    assert(file.is_open());
    file.seekg(p * pageSize);
    file.read(reinterpret_cast<char*> (buffer), pageSize);
}

template<typename w>
bool Storage<w>::open() {
    if(!file.is_open())
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    return file.is_open();
}

#endif //BITVECTOR_STORAGE_H
