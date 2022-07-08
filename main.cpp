#include <iostream>
#include <cstdlib>
#include "ExternalBitvect.h"

using namespace std;

int main() {
    cout << "Size of uint64_t = " << sizeof(uint64_t) << endl;
    uint64_t buffer[10];
    uint64_t bufferRead[10]{0};

    //LLenamos un buffer con basura
    for (int i = 0; i < 10; i++){
        buffer[i] = UINT32_MAX;
        cout << hex << "buffer[" << i << "] = " << buffer[i] << endl;
    }

    // Objetos a probar (solo access en bitvector)
    ExternalBitvector<uint64_t> pruebaVector("Testing.bin", 10);
    Storage<uint64_t>* pruebaStorage = pruebaVector.getStorage();

    // LLenamos 10 paginas con basura
    for(int i = 0; i < 10; i++){
        pruebaStorage->appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    pruebaStorage->readPage(bufferRead, 6);
    for (int i = 0; i < 10; i++){
        cout << "bufferRead[" << i << "] = " << hex << bufferRead[i] << dec << endl;
    }

    // Accedemos a los primeros 256 bits
    for(int i = 0; i < 256; i++)
        cout << "prueaVector[" << i+1 << "] = " << hex << pruebaVector[i+1] << dec << endl;

    // Accedemos a 10 bits randoms en el rango de 0 a 10*pageSize*8
    for(int i = 0; i < 10; i++){
        uint64_t accessBit = rand()% (10*10*sizeof(uint64_t)*8);
        cout << "pruebaVector[" << accessBit << "] = " << pruebaVector[accessBit] << endl;
    }

    return 0;
}
