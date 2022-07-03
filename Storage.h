//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_STORAGE_H
#define BITVECTOR_STORAGE_H

#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

template<typename T>
class Storage {
private:
    string fileName;

    int pageSize;

    fstream file;
public:
    Storage(string name, int D);

    int appendPage(const T buffer[]);

    int writePage(const T buffer[], int p);

    int readPage(T buffer[], int p);

    int open();

    void close();

};

template<typename T>
Storage<T>::Storage(string fileName, int D) {
    ofstream crearArchivo(fileName);
    if(!crearArchivo.is_open()){
        cout << "Error" << endl;
    }
    crearArchivo.close();
    Storage::fileName = fileName;
    Storage::pageSize = D * sizeof(T);
}

template<typename T>
int Storage<T>::appendPage(const T buffer[]) {
    file.seekp(0, ios::end);
    file.write((char *) (buffer), pageSize);
    return 0;
}

template<typename T>
int Storage<T>::writePage(const T buffer[], int p) {
    file.seekp(0, ios::end);
    if (p >= file.tellp()/pageSize) {
        return -1;
    }
    file.seekp(p * pageSize);
    file.write((char *) (buffer), pageSize);
    return 0;
}

template<typename T>
int Storage<T>::readPage(T buffer[], int p) {
    file.seekg(0, ios::end);
    if (p >= file.tellg()/pageSize) {
        return -1;
    }
    file.seekg(p * pageSize);
    file.readsome((char *) (buffer), pageSize);
    return 0;
}


//modes: r, read; w: write, rw:read and write, a: append
template<typename T>
int Storage<T>::open() {
    file.open(fileName, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        return -1;
    }
    return 0;
}

template<typename T>
void Storage<T>::close() {
    file.close();

}

#endif //BITVECTOR_STORAGE_H
