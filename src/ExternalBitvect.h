//
// Created by Carlos on 06-07-2022.
//

#ifndef PRUEBASC___EXTERNALBITVECT_H
#define PRUEBASC___EXTERNALBITVECT_H

#include <string>
#include <filesystem>
#include "BitBasic.h"
#include "Storage.h"

using namespace std;


template<typename uint_t>
class ExternalBitvector {
private:
    unsigned long long length = 0;//in bits
    unsigned long long ones=0; //in bits
    Storage<uint_t> &theStorage;
public:
    ExternalBitvector(Storage<uint_t> &theStorage);

    ~ExternalBitvector();

    void create(const unsigned long long len);
    void create();
    void open();
    //buffer to append, n <= bitPerBuffer bits to append in bitvector.
    void appendPage(Page<uint_t> buffer, uint_t n);
    void resize(const uint_t new_length);

    int operator[](const uint_t b) const;

    int access(const uint_t b) const;

    void bitSet(const uint_t b);

    void bitClean(const uint_t b);

    uint_t rank(const uint_t b) const;

    void buildRank();

    unsigned long long getLength() {
        return length;
    }

    void print(int with = 128);

private:
    void apendBlank(uint_t N);

    inline tuple<Page<uint_t>, uint_t, uint_t> getBufferPageAndPos(const uint_t b) const;
};

template<typename uint_t>
ExternalBitvector<uint_t>::ExternalBitvector(Storage<uint_t> &storage):theStorage(storage) {
    length=0;
    ones=0;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string(length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string(ones));
}

template<typename uint_t>
ExternalBitvector<uint_t>::~ExternalBitvector() {
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string(length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string(ones));
    theStorage.close();
}

template<typename uint_t>
void ExternalBitvector<uint_t>::create(const unsigned long long len) {
    assert(len>0);
    theStorage.create();
    open();
    resize(len);
    assert((len / theStorage.bitsPerPage() <
            theStorage.getPages()));// "No se crea correctamente el 'ExternalBitVector'"
}

template<typename uint_t>
void ExternalBitvector<uint_t>::apendBlank(const uint_t N) {
    uint_t pageSize = theStorage.pageSize() * 8;//page size in bits.
    Page<uint_t> buffer(theStorage.getD());
    buffer.createBlock(true);//true para crear el buffer limpio
    for (uint_t i = 0; i < N; i++) {
        theStorage.appendPage(buffer);
    }
    length+=N;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string(length));
}

template<typename uint_t>
void ExternalBitvector<uint_t>::open() {
    theStorage.open();
    length = stoull(theStorage.getMeta("EXTERNAL_BIT_VECTOR_LENGTH"));
    ones = stoull(theStorage.getMeta("EXTERNAL_BIT_VECTOR_ONES"));
}

template<typename uint_t>
void ExternalBitvector<uint_t>::resize(uint_t new_length) {
    if (new_length == length) return;
    //si el nuevo valor es menor que length, borrar bloques de la cola, sino agregar.
    if (new_length > length) {
        //se agregan páginas
        uint_t pageSize = theStorage.pageSize() * 8;//page size in bits.
        uint_t pageNumber = (new_length - length + pageSize - 1) / pageSize;
        apendBlank(pageNumber);

    } else {
        static_assert(true, "Falta implementar la eliminación de páginas cuando 'resize' achica el bitvector");
    }
    length = new_length;
}

//RETURN buffer, page number and pos in buffer
template<typename uint_t>
inline tuple<Page<uint_t>, uint_t, uint_t> ExternalBitvector<uint_t>::getBufferPageAndPos(const uint_t b) const {
    assert(b >= 0 && b < length);
    uint_t bitsPerPage = theStorage.bitsPerPage();
    Page<uint_t> buffer(theStorage.getD());
    uint_t page = b / bitsPerPage;
    uint_t pos = b % bitsPerPage;
    buffer = theStorage.readPage(page);
    return make_tuple(buffer, page, pos);
}

template<typename uint_t>
int ExternalBitvector<uint_t>::access(const uint_t b) const {
    Page<uint_t> buffer(theStorage.getD()); uint_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);
    return (BitBasic::access(buffer.data(), pos, theStorage.bitsPerWord()))?1:0;
}

template<typename uint_t>
int ExternalBitvector<uint_t>::operator[](const uint_t b) const {
    return access(b);
}

template<typename uint_t>
void ExternalBitvector<uint_t>::bitSet(const uint_t b) {
    Page<uint_t> buffer(theStorage.getD()); uint_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);

    BitBasic::BitSet(buffer.data(), pos, theStorage.bitsPerWord());
    theStorage.updatePage(buffer, page);
}

template<typename uint_t>
void ExternalBitvector<uint_t>::bitClean(const uint_t b) {
    Page<uint_t> buffer(theStorage.getD()); uint_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);

    BitBasic::BitClear(buffer.data(), pos, theStorage.bitsPerWord());
    theStorage.updatePage(buffer, page);
}

template<typename uint_t>
uint_t ExternalBitvector<uint_t>::rank(const uint_t b) const {
    Page<uint_t> buffer(theStorage.getD()); uint_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);

    uint_t prevRank = buffer[theStorage.getD() - 1];
    return prevRank + BitBasic::Rank(buffer.data(), pos, theStorage.bitsPerWord());
}
template<typename uint_t>
void ExternalBitvector<uint_t>::appendPage(Page<uint_t> buffer, uint_t n) {
    //append buffer
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = theStorage.bitsPerWord();
    assert(n<=bitsPerPage);
    buffer[theStorage.getD() - 1] = ones;
    theStorage.appendPage(buffer);
    ones+=BitBasic::Rank(buffer.data(), n - 1, bitsPerWord);
    length+=n;
    //quizas solo debería guardarse en el storage y rescatarlo con funciones getter y setter
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string(length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string(ones));
}

template<typename uint_t>
void ExternalBitvector<uint_t>::buildRank() {
    uint_t paginas = theStorage.getPages();
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = theStorage.bitsPerWord();
    Page<uint_t> buffer(theStorage.getD());
    uint_t prevRank = 0;
    uint_t ori = 0;
    for (uint_t i = 0; i < paginas; i++) {
        buffer = theStorage.readPage(i);
        buffer[theStorage.getD() - 1] = prevRank;
        prevRank += BitBasic::Rank(buffer.data(), bitsPerPage - 1, bitsPerWord);
        theStorage.updatePage(buffer, i);
    }
    ones=prevRank;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string(ones));
}

template<typename uint_t>
void ExternalBitvector<uint_t>::print(int with) {
    //print ExternalBitVector
    cout << "(EXTERNAL_BIT_VECTOR_LENGTH " << length << ")" << endl;
    cout << "(EXTERNAL_BIT_VECTOR_ONES " << ones <<  ") only if builrank() was run!" << endl;
    for (int i = 4; i <= with; i += 4) {
        cout << fixed << setw(4) << setfill(' ') << std::right << i << " ";
    }
    cout << endl;
    for (uint_t i = 0; i < length; i++) {
        cout << access(i);
        if ((i + 1) % 4 == 0)cout << " ";
        if (((i + 1) % with) == 0) {
            cout << "  " << (i + 1) / with << endl;
        }
    }
    cout << endl;
}

template<typename uint_t>
void ExternalBitvector<uint_t>::create() {
    theStorage.create();
    open();
    length=0;
    ones=0;
}


#endif //PRUEBASC___EXTERNALBITVECT_H
