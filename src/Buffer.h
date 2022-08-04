//
// Created by ThinkBook14G2 on 10-07-2022.
//

#ifndef PRUEBASC___BUFFER_H
#define PRUEBASC___BUFFER_H


#include <memory>

using namespace std;

template<typename uint_t>
class Buffer {
private:
    shared_ptr<uint_t[]> block= nullptr;
    uint_t D=8;
    bool updated=false;
    uint_t id=-1;

public:
    explicit Buffer(uint_t d) : D(d), block(nullptr){ }

    void createBlock(bool blank=false){
        if(blank){
            block= shared_ptr<uint_t[]>(new uint_t[D]{0});
        }else{
            block= shared_ptr<uint_t[]>(new uint_t[D]);
        }
    }
    void setChunk(uint32_t p, uint8_t chunk){
        uint_t * bitArray=block.get();
        //solo chunk útiles para datos (se excluye el word para rank)
        uint32_t chunkPerWord=sizeof(uint_t)*2;
        //incluye el word para rank
        uint32_t bitsPerWord=sizeof(uint_t)*8;
        int i = p / chunkPerWord;
        int j = p % chunkPerWord;
        uint_t mask = chunk;
        bitArray[i]|=mask << bitsPerWord-4-(4*j);
        cout<<"unt_t:"<< i<<" chunk:"<<j<<" "<<endl;
    }
    Buffer(const Buffer &other){
        //share ptr, not copy
        block=other.block;
        D=other.D;
        updated=other.updated;
        id=other.id;
    }
    uint_t& operator=(const uint_t& other){
        block=other.block;
        D=other.D;
        updated=other.updated;
        id=other.id;
    }

    uint_t& operator[](uint_t b) const{
        return block[b];
    }

    bool operator==(const Buffer &rhs) const {
        return block == rhs.block &&
               D == rhs.D &&
               updated == rhs.updated && id=rhs.id;
    }

    bool operator!=(const Buffer &rhs) const {
        return !(rhs == *this);
    }

    uint_t * data() {
        return block.get();
    }

    uint_t getD() const {
        return D;
    }


    bool isUpdated(){
        return updated;
    }

    void setUpdated(){
        updated=true;
    }

    uint_t getId() const {
        return id;
    }

    void setId(uint_t id) {
        Buffer::id = id;
    }

};


#endif //PRUEBASC___BUFFER_H
