//
// Created by Carlos on 06-07-2022.
//

#ifndef PRUEBASC___BITVECT_H
#define PRUEBASC___BITVECT_H

#include <filesystem>
#include "Storage.h"

namespace fs = std::filesystem;

template<typename w>
class Bitvector {
private:
    int pageSize;
    Storage<w>* theStorage;
    w* bufferPage;
public:
    Bitvector(std::string name, int D);

    ~Bitvector();

    int operator[](uint64_t b);

    uint64_t rank(uint64_t b);

    Storage<w>* getStorage();
};

template<typename w>
Bitvector<w>::Bitvector(std::string name, int D){
    if(!fs::exists(name)){
        std::ofstream createFile(name);
        assert(createFile.is_open()); // cambiar por excepción
        createFile.close();
    }
    pageSize = D*sizeof(w);
    bufferPage = new w[pageSize];
    theStorage = new Storage<w>(name, pageSize);
    theStorage->open();
}

template<typename w>
Bitvector<w>::~Bitvector(){
    delete theStorage;
    delete bufferPage;
}

template<typename w>
int Bitvector<w>::operator[](uint64_t b){
    uint64_t bitsPerPage = 8*pageSize;
    uint64_t bitsPerWord = 8*sizeof(w);
    assert(b > 0 && b < (theStorage->getPages()) * bitsPerPage);
    b--;

    /* BORRAR */
    //std::cout << std::dec << "Pagina a obtener el bit: " << b << "/(" << bitsPerPage;
    //std::cout << std::dec << ") = " << b/bitsPerPage << std::endl;
    /* BORRAR */

    theStorage->readPage(bufferPage, b/bitsPerPage);

    /* BORRAR */
    //std::cout << std::dec << "Palabra donde se encuentra el bit: " << "(" << b << "%(" << bitsPerPage << "))/(";
    //std::cout << std::dec << bitsPerWord << ") = " << (b%bitsPerPage)/(bitsPerWord) << std::endl;
    /* BORRAR */

    w& word= bufferPage[(b%bitsPerPage)/(bitsPerWord)];

    // si access se inicializa en 1 << (epresión) falla.
    w access = 1;
    access <<= ((bitsPerWord-1)-b%bitsPerWord); // asume que el bit menos significativo esta mas a la derecha.
    access &= word;
    if(access>0)
        return 1;
    else
        return 0;
}

template<typename w>
Storage<w>* Bitvector<w>::getStorage(){
    return theStorage;
}

#endif //PRUEBASC___BITVECT_H
