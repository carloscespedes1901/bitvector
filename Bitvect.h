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

    /* Comentar la explicación mas detallada */
    std::cout << "Pagina a obtener el bit: " << b << "/(8*" << pageSize;
    std::cout << ") = " << b/(8*pageSize) << std::endl;
    /* Comentar la explicación mas detallada */

    theStorage->readPage(bufferPage, b/(8*pageSize));

    /* Comentar la explicación mas detallada */
    std::cout << "Palabra donde se encuentra el bit: " << "(" << b << "%(8*" << pageSize << "))/(8*";
    std::cout << sizeof(w) << ") = " << (b%(8*pageSize))/(8*sizeof(w)) << std::endl;
    /* Comentar la explicación mas detallada */

    w& word= bufferPage[(b%(8*pageSize))/(8*sizeof(w))];

    /* Comentar la explicación mas detallada */
    std::cout << "bit de la palabra donde se encuentra el bit: " << b << "%(8*" << sizeof(w) << ") = ";
    std::cout << b%(8*sizeof(w)) << std::endl;
    /* Comentar la explicación mas detallada */

    w access = 1;
    access <<= (8*sizeof(w)-1); // si se inicializa 1 << 8*sizeof(w)-1 hay problemas pues toma el 1 como int 1
    access >>= (b%(8*sizeof(w)));
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
