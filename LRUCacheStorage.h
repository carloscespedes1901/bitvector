//
// Created by ThinkBook14G2 on 08-07-2022.
//

#ifndef PRUEBASC___LRUCACHESTORAGE_H
#define PRUEBASC___LRUCACHESTORAGE_H

#include <list>
#include <unordered_map>
#include "Storage.h"

template<typename uint_t>
class LRUCacheStorage : Storage<uint_t> {
private:
    // store page of cache
    list<Buffer<uint_t>> pageList;
    // store references of page in pageList of key in cache
    unordered_map<int, typename list<Buffer<uint_t>>::iterator> map;
    int cacheSize; // maximum capacity of cache

public:
    LRUCacheStorage(const string &fileName, int d, int csize) : Storage<uint_t>(fileName, d), cacheSize(csize) {}

    void appendPage(const Buffer<uint_t> buffer) override;

    void updatePage(Buffer<uint_t> buffer, uint_t p) override;

    Buffer<uint_t> readPage(uint_t p) override;

private:
    inline void deleteLRU();

    inline void updateAndMoveToFront(const Buffer<uint_t> &buffer, uint_t p);

    inline void addNewCacheEntry(const Buffer<uint_t> &buffer, uint_t p);

    inline bool cacheContains(uint_t p);

    inline bool cacheIsFull();

    inline Buffer<uint_t> moveToFront(uint_t p) const;
};


#endif //PRUEBASC___LRUCACHESTORAGE_H
