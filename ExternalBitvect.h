//
// Created by Carlos on 06-07-2022.
//

#ifndef PRUEBASC___EXTERNALBITVECT_H
#define PRUEBASC___EXTERNALBITVECT_H

#include <filesystem>
#include "Storage.h"
#include <string>
#include "BitBasic.h"

using namespace std;


template<typename uint_t>
class ExternalBitvector {
private:
    unsigned long long length;//in bits
    Storage<uint_t> theStorage;
public:
    ExternalBitvector(Storage<uint_t> theStorage);

    ~ExternalBitvector();

    void create(uint_t length);

    void open();

    void resize(uint_t new_length);

    int operator[](uint_t b);

    int access(uint_t b);

    void bitSet(uint_t b);

    void bitClean(uint_t b);

    uint_t rank(uint_t b);

    void buildRank();

    void apendBlank(uint_t N);
};

template<typename uint_t>
ExternalBitvector<uint_t>::ExternalBitvector(Storage<uint_t> storage):theStorage(storage) {}

template<typename uint_t>
ExternalBitvector<uint_t>::~ExternalBitvector() {

}

template<typename uint_t>
void ExternalBitvector<uint_t>::create(uint_t length) {
    theStorage.create();
    //calculate number of page for length bits
    uint_t pageSize = theStorage.pageSize() * 8;//page size in bits.
    uint_t pageNumber = (length + pageSize - 1) / pageSize;
    apendBlank(pageNumber);
    theStorage.addMeta("length", to_string(length));
}

template<typename uint_t>
void ExternalBitvector<uint_t>::apendBlank(uint_t N) {
    uint_t pageSize = theStorage.pageSize() * 8;//page size in bits.
    uint_t buffer[theStorage.getD()]{0};
    for (uint_t i = 0; i < N; i++) {
        theStorage.appendPage(buffer);
    }
}

template<typename uint_t>
void ExternalBitvector<uint_t>::open() {
    theStorage.open();
    length=stoull(theStorage.getMeta("length"));
}

template<typename uint_t>
void ExternalBitvector<uint_t>::resize(uint_t new_length) {
    if(new_length==length) return;
    //si el nuevo valor es menor que length, borrar bloques de la cola, sino agregar.
    if (new_length>length){
        //se agregan páginas
        uint_t pageSize = theStorage.pageSize() * 8;//page size in bits.
        uint_t pageNumber = (new_length-length + pageSize - 1) / pageSize;
        apendBlank(pageNumber);
        theStorage.addMeta("length", to_string(length));
    }else{
        //falta ver como eliminar páginas.
    }
}

template<typename uint_t>
int ExternalBitvector<uint_t>::access(uint_t b) {
    assert(b >= 0 && b < length);
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = 8 * sizeof(uint_t);

    uint_t bufferPage[theStorage.getD()];
    theStorage.readPage(bufferPage, b / bitsPerPage);
    uint_t pos=b%bitsPerPage;
    return BitBasic::access(bufferPage, pos, bitsPerWord);

}

template<typename uint_t>
int ExternalBitvector<uint_t>::operator[](uint_t b) {
    return access(b);
}

template<typename uint_t>
void ExternalBitvector<uint_t>::bitSet(uint_t b) {
    assert(b >= 0 && b < length);
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = 8 * sizeof(uint_t);

    uint_t bufferPage[theStorage.getD()];
    int page=b/bitsPerPage;
    theStorage.readPage(bufferPage, page);
    uint_t pos=b%bitsPerPage;
    BitBasic::BitSet(bufferPage, pos, bitsPerWord);
    theStorage.writePage(bufferPage, page);
}

template<typename uint_t>
void ExternalBitvector<uint_t>::bitClean(uint_t b) {
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = 8 * sizeof(uint_t);
    assert(b >= 0 && b < length);
    uint_t bufferPage[theStorage.getD()];
    int page=b/bitsPerPage;
    theStorage.readPage(bufferPage, b / bitsPerPage);
    uint_t pos=b%bitsPerPage;
    BitBasic::BitClear(bufferPage, pos, bitsPerWord);
    theStorage.writePage(bufferPage, page);
}

template<typename uint_t>
uint_t ExternalBitvector<uint_t>::rank(uint_t b) {
    assert(b >= 0 && b < length);

    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = 8 * sizeof(uint_t);
    uint_t bufferPage[theStorage.getD()];
    int page=b/bitsPerPage;
    if(theStorage.getPages() < page) resize(page);
    theStorage.readPage(bufferPage, b / bitsPerPage);
    uint_t pos=b%bitsPerPage;
    uint_t rank=bufferPage[theStorage.getD()-1];
    return rank+BitBasic::Rank(bufferPage, pos, bitsPerWord);

}

template<typename uint_t>
void ExternalBitvector<uint_t>::buildRank() {
    int paginas=theStorage.getPages();
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = 8 * sizeof(uint_t);
    uint_t bufferPage[theStorage.getD()];
    for(int i=0;i<paginas;i++){
        theStorage.readPage(bufferPage, i / bitsPerPage);
        bufferPage[theStorage.getD()-1]=BitBasic::Rank(bufferPage, bitsPerPage-1, bitsPerWord);
        theStorage.writePage(bufferPage, i);
    }
}


#endif //PRUEBASC___EXTERNALBITVECT_H
