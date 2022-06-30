//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_STORAGE_H
#define BITVECTOR_STORAGE_H

#include <string>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

template<typename T>
class Storage {
private:
    string fileName;
    T pages;
    int pageSize;
    fstream file;
public:
    Storage(string name, int D);

    Storage(string name);

    int getNumberOfPages();

    int appendPage(const T buffer[]);

    int writePage(const T buffer[], int p);

    int readPage(T buffer[], int p);

    int open(string mode);

    void close();

};

template<typename T>
Storage<T>::Storage(string fileName, int D) {
    Storage::fileName = fileName;
    Storage::pages = 0;
    Storage::pageSize = D * sizeof(T);
}

template<typename T>
int Storage<T>::appendPage(const T buffer[]) {
    file.write((char *) (buffer), pageSize);
    Storage::pages++;
    return 0;
}

template<typename T>
int Storage<T>::writePage(const T buffer[], int p) {
    if (p >= Storage::pages) {
        return -1;
    }
    file.seekp(p * pageSize);
    file.write((char *) (buffer), Storage::pageSize);
    return 0;
}

template<typename T>
int Storage<T>::readPage(T buffer[], int p) {
    if (p >= Storage::pages) {
        return -1;
    }
    file.seekg(p * pageSize);
    file.readsome((char *) (buffer), Storage::pageSize);
    return 0;
}

template<typename T>
int Storage<T>::getNumberOfPages() { return Storage::pages; }

//modes: r, read; w: write, rw:read and write, a: append
template<typename T>
int Storage<T>::open(string mode) {
    ios_base::openmode __mode = ios::binary;

    if (mode.compare("r")) {
        __mode |= ios::in;
    } else if (mode.compare("w")) {
        __mode |= ios::out;
    } else if (mode.compare("rw")) {
        __mode |= ios::out | ios::in;
    } else if (mode.compare("a")) {
        __mode |= ios::ate;
    } else {
        return -1;
    }
    file.open(fileName, __mode);
    if (!file.is_open()) {
        return -1;
    }
    const auto begin = file.tellg();
    file.seekg(0, ios::end);
    const auto end = file.tellg();
    const auto fsize = (end - begin);
    pages = fsize / pageSize;
    return 0;
}

template<typename T>
void Storage<T>::close() {
    file.close();

}

#endif //BITVECTOR_STORAGE_H
