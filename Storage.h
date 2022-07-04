//
// Created by Carlos on 29-06-2022.
//

#ifndef BITVECTOR_STORAGE_H
#define BITVECTOR_STORAGE_H

#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

template<typename w>
class Storage {
private:
    string fileName;

    int pageSize;

    fstream file;

public:
    Storage(string name, int D);

    ~Storage();

    int appendPage(const w buffer[]);

    int writePage(const w buffer[], int p);

    int readPage(w buffer[], int p);

    int open();

    void close();
};

template<typename w>
Storage<w>::Storage(string fileName, int D) {
    if(!fs::exists(fileName)){
        ofstream crearArchivo(fileName);
        if(!crearArchivo.is_open()){
            cout << "Error al crear archivo" << endl;
        }
        crearArchivo.close();
    }
    Storage::fileName = fileName;
    Storage::pageSize = D * sizeof(w);
}

template<typename w>
Storage<w>::~Storage() {
    close();
}

template<typename w>
int Storage<w>::appendPage(const w buffer[]) {
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*> (buffer), pageSize);
    return 0;
}

template<typename w>
int Storage<w>::writePage(const w buffer[], int p) {
    file.seekp(0, ios::end);
    if (p >= file.tellp()/pageSize) {
        return -1;
    }
    file.seekp(p * pageSize);
    file.write(reinterpret_cast<const char*> (buffer), pageSize);
    return 0;
}

template<typename w>
int Storage<w>::readPage(w buffer[], int p) {
    file.seekg(0, ios::end);
    if (p >= file.tellg()/pageSize) {
        return -1;
    }
    file.seekg(p * pageSize);
    file.read(reinterpret_cast<char*> (buffer), pageSize);
    return 0;
}

template<typename w>
int Storage<w>::open() {
    file.open(fileName, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        return -1;
    }
    return 0;
}

template<typename w>
void Storage<w>::close() {
    file.close();

}

#endif //BITVECTOR_STORAGE_H
