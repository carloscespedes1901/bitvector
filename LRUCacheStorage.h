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
    list<tuple<int, uint_t *>> pageList;
    // store references of page in pageList of key in cache
    unordered_map<int, typename list<tuple<int, uint_t *>>::iterator> map;
    int csize; // maximum capacity of cache

public:
    LRUCacheStorage(const string &fileName, int d, int csize) : Storage<uint_t>(fileName, d), csize(csize) {}

    void appendPage(const uint_t *buffer) override ;

    void updatePage(const uint_t *buffer, uint_t p) override ;

    void readPage(uint_t *&buffer, uint_t p) override ;
};


#endif //PRUEBASC___LRUCACHESTORAGE_H
