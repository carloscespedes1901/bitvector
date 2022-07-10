#include <iostream>
#include <cstdlib>
#include "ExternalBitvect.h"

using namespace std;
void test_Storage(){
    cout << "Size of uint64_t = " << sizeof(uint64_t) << endl;
    uint64_t *buffer = new uint64_t[10];
    uint64_t *bufferRead = new uint64_t[10]{0};

    //LLenamos un buffer con basura
    for (int i = 0; i < 10; i++) {
        buffer[i] = UINT32_MAX;
        cout << hex << "buffer[" << i << "] = " << buffer[i] << endl;
    }
    Storage<uint64_t> pruebaStorage("pruebaStorage3.bin", 10);
    pruebaStorage.open();
    // LLenamos 10 paginas con basura
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    pruebaStorage.readPage(bufferRead, 6);
    for (int i = 0; i < 10; i++) {
        cout << "bufferRead[" << i << "] = " << hex << bufferRead[i] << dec << endl;
    }

    delete[] buffer;
    delete[] bufferRead;


}

void test_ExternalBitVector(){
    // Objetos a probar (solo access en bitvector)
    Storage<uint64_t> vectorStorage("Testing.bin", 10);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);

    pruebaVector.create(1000);
    for (int i = 0; i < 1000; i += 4) {
        uint64_t pos=i;
        pruebaVector.bitSet(pos);
    }
    for (int i = 0; i < 1000; i++) {
        cout << pruebaVector[i];
        if ((i % 99) == 0) {
            cout << endl;
        }
    }
}
int main() {
    test_Storage();
    test_ExternalBitVector();
}
