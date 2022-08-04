#include <iostream>
#include "Storage.h"
#include "LRUCacheStorage.h"
#include "ExternalBitvect.h"
#include "ExternalK2Tree.h"

using namespace std;

void test_Storage() {
    cout << "Size of uint64_t = " << sizeof(uint64_t) << endl;

    Buffer<uint64_t> buffer(10);
    buffer.createBlock();
    Buffer<uint64_t> bufferRead(10);
    bufferRead.createBlock(true);
    //LLenamos un buffer con basura
    for (int i = 0; i < 10; i++) {
        buffer[i] = UINT32_MAX;
        cout << hex << "buffer[" << i << "] = " << buffer[i] << endl;
    }
    Storage<uint64_t> pruebaStorage("pruebaStorage.bin", 10);
    pruebaStorage.create();//lo crea solo sino existe;
    pruebaStorage.open();
    // LLenamos 10 paginas con basura
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);
    for (int i = 0; i < 10; i++) {
        cout << "bufferRead[" << i << "] = " << hex << bufferRead[i] << dec << endl;
    }
}

void test_ExternalBitVector() {
    // Objetos a probar (solo access en bitvector)
    //no me gusta que el objeto quede libre por fuera
    Storage<uint64_t> vectorStorage("Testing.bin", 10);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);

    pruebaVector.create(1000);
    cout << "bits en 1: 512-515" << endl;
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);
    pruebaVector.print(100);
    cout
            << "Print es basado en acces. La siguiente prueba evalua access individual de los bits en 1 512-515 (se espera 1 1 1 1 )"
            << endl;
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;
    cout << "se limpian los 4 bits anteriores y se revisan con rank se espera (0 0 0 0)" << endl;
    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;

    cout << "poniendo en 1 cada 4 bits desde el 1°" << endl;
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4) {
        pruebaVector.bitSet(i);
    }
    uint64_t pos = 0;
    pruebaVector.print();

    cout << "prueba de build rank y rank (se espera 250)" << endl;
    pruebaVector.buildRank();
    cout << pruebaVector.rank((uint64_t) 999) << endl;

}

void test_bits() {
    uint64_t bits = 0, i = 0;
    int bit;
    cout << ((BitBasic::access(&bits, (uint64_t) 0, 64)) ? 1 : 0) << endl;
    cout << "set(0)" << endl;
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    cout << ((BitBasic::access(&bits, (uint64_t) 0, 64)) ? 1 : 0) << endl;
    for (uint64_t i = 0; i < 64; i++) {
        bit = (BitBasic::access(&bits, i, 64) ? 1 : 0);
        cout << bit;
    }
    cout << endl;
    cout << bitset<64>(bits) << endl;

    i = 2;
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;
    cout << "set(2)" << endl;
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;

    for (int i = 0; i < 64; i++) {
        cout << (BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0;
    }
    cout << endl;

    cout << bitset<64>(bits) << endl;

    i = 4;
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;
    cout << "set(4)" << endl;
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;

    for (int i = 0; i < 64; i++) {
        cout << ((BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0);
    }
    cout << endl;
    cout << bitset<64>(bits) << endl;
    BitBasic::BitClear(&bits, (uint64_t) 0, 64);
    BitBasic::BitClear(&bits, (uint64_t) 2, 64);
    BitBasic::BitClear(&bits, (uint64_t) 4, 64);
    for (int i = 0; i < 64; i++) {
        cout << ((BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0);
    }
    cout << endl;
    cout << bitset<64>(bits) << endl;
    cout << endl;
    BitBasic::BitSet(&bits, (uint64_t) 10, 64);
    BitBasic::BitSet(&bits, (uint64_t) 11, 64);
    BitBasic::BitSet(&bits, (uint64_t) 12, 64);
    BitBasic::BitSet(&bits, (uint64_t) 13, 64);
    cout << bitset<64>(bits) << endl;
    for (int i = 0; i < 64; i++) {
        cout << ((BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0);
    }
    cout << endl;
    i = 13;
    cout << "rank(13)= " << BitBasic::Rank(&bits, i, 64) << "(esperado 4) " << endl;
}

void test_ExternalBitVectorWithLRU() {
    // Objetos a probar (solo access en bitvector)
    //no me gusta que el objeto quede libre por fuera
    //Storage<uint64_t> vectorStorage("Testing.bin", 10);
    LRUCacheStorage<uint64_t> vectorStorage("Testing.bin", 10, 20);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);

    pruebaVector.create(1000);
    cout << "bits en 1: 512-515" << endl;
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);
    pruebaVector.print(100);
    cout
            << "Print es basado en acces. La siguiente prueba evalua access individual de los bits en 1 512-515 (se espera 1 1 1 1 )"
            << endl;
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;
    cout << "se limpian los 4 bits anteriores y se revisan con rank se espera (0 0 0 0)" << endl;
    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;

    cout << "poniendo en 1 cada 4 bits desde el 1°" << endl;
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4) {
        pruebaVector.bitSet(i);
    }
    uint64_t pos = 0;
    pruebaVector.print();

    cout << "prueba de build rank y rank (se espera 250)" << endl;
    pruebaVector.buildRank();
    cout << pruebaVector.rank((uint64_t) 999) << endl;

}
string intVec2Str(vector<unsigned long> vec){
    string output;
    output.append("[ ");
    for(auto i=vec.begin();i!=vec.end();++i){
        output.append(to_string(*i));
        output.append(" ");

    }
    output.append("]");
    return output;
}
void testExternalK2tree() {
    cout<<"creando el k2tree externo"<<endl;
    LRUCacheStorage<uint64_t> cacheStorage("../k2tree.bin", 10, 20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, cacheStorage);
    externalK2Tree.create("../../dataset/uniform16x16.txt");
    cout<<"abriendo el k2tree externo"<<endl;
    externalK2Tree.open();


    cout<<"Matriz de adyacencia construida con check link..."<<endl;
    //check link
    cout<<endl<< "    ";
    for(int i=0;i<16;i++){
        cout <<(i<10?"  ":" ")<< i<<" ";
    }
    cout<<endl<< "   ";
    for(int i=0;i<16;i++){
        cout <<"____";
    }
    cout<<endl;
    for(int i=0;i<16;i++){
        cout <<((i<10)?" ":"")<<i<< "| ";
        for(int j=0;j<16;j++){
            cout <<"  "<< externalK2Tree.checkLink(i,j) <<" ";
        }
        cout <<endl;
    }
    cout<<"test de cada punto con check link..."<<endl;
    cout << "(1,1) = " << externalK2Tree.checkLink(1, 1) << endl;
    cout << "(1,3) = " << externalK2Tree.checkLink(1, 3) << endl;
    cout << "(1,7) = " << externalK2Tree.checkLink(1, 7) << endl;
    cout << "(1,11) = " << externalK2Tree.checkLink(1, 11) << endl;
    cout << "(4,11) = " << externalK2Tree.checkLink(4, 11) << endl;
    cout << "(4,13) = " << externalK2Tree.checkLink(4, 13) << endl;
    cout << "(4,15) = " << externalK2Tree.checkLink(4, 15) << endl;
    cout << "(5,2) = " << externalK2Tree.checkLink(5, 2) << endl;
    cout << "(6,14) = " << externalK2Tree.checkLink(6, 14) << endl;
    cout << "(10,4) = " << externalK2Tree.checkLink(10, 4) << endl;
    cout << "(11,11) = " << externalK2Tree.checkLink(11, 11) << endl;
    cout << "(13,7) = " << externalK2Tree.checkLink(13, 7) << endl;
    cout << "(14,3) = " << externalK2Tree.checkLink(14, 3) << endl;
    cout << "(15,7) = " << externalK2Tree.checkLink(15, 7) << endl;
    cout << "(15,15) = " << externalK2Tree.checkLink(15, 15) << endl;

    cout << "(6,15) Esperado false=" << externalK2Tree.checkLink(6, 15) << endl;
    cout<<"Direct and Reverse Neighbors..."<<endl;
    cout << "Direct Neighbors of 4: Esperado: [ 11 13 15 ] Obtenido: "<< intVec2Str(externalK2Tree.direct(4))<<endl;
    cout << "Direct Neighbors of 1: Esperado: [ 1 3 7 11 ] Obtenido: "<< intVec2Str(externalK2Tree.direct(1))<<endl;
    cout << "Direct Neighbors of 11: Esperado: [ 11 ] Obtenido: "<< intVec2Str(externalK2Tree.direct(11))<<endl;
    cout << "Direct Neighbors of 12: Esperado: [ ] Obtenido: "<< intVec2Str(externalK2Tree.direct(12))<<endl;

    cout << "Reverse Neighbors of 7:  Esperado: [ 1 13 15 ] Obtenido: "<< intVec2Str(externalK2Tree.reverse(7))<<endl;
    cout << "Reverse Neighbors of 11: Esperado: [ 1 4 11 ] Obtenido: "<< intVec2Str(externalK2Tree.reverse(11))<<endl;
    cout << "Reverse Neighbors of 5:  Esperado: [  ] Obtenido: "<< intVec2Str(externalK2Tree.reverse(5))<<endl;
    cout<<" Range query"<<endl;
    auto nodes=externalK2Tree.getNodes();

    auto output=externalK2Tree.range(0,nodes-1,0,nodes-1);
    //save output
    ofstream outfile;
    outfile.open("../rangeOutput.txt");
    for (const auto &point: output) {
        outfile<<point.first<<" "<<point.second<<endl;
    }
    outfile.close();



    externalK2Tree.close();

}

int main() {

 //   test_bits();

  //  test_Storage();

 //   test_ExternalBitVector();

 //   test_ExternalBitVectorWithLRU();

    testExternalK2tree();
}
