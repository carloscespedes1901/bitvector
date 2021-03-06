//
// Created by ThinkBook14G2 on 08-07-2022.
//

#ifndef PRUEBASC___LRUCACHESTORAGE_H
#define PRUEBASC___LRUCACHESTORAGE_H

#include <list>
#include <unordered_map>
#include "Storage.h"

template<typename uint_t>
class LRUCacheStorage : public Storage<uint_t> {
private:
    // store page of cache

    list <Buffer<uint_t>> pageList;
    // store references of page in pageList of key in cache
    unordered_map<uint_t, typename list<Buffer<uint_t>>::iterator> map;
    int cacheSize; // maximum capacity of cache

public:
    LRUCacheStorage(const string &fileName, int d, int csize) : Storage<uint_t>(fileName, d), cacheSize(csize) {}

    virtual void appendPage(Buffer<uint_t> buffer) override;

    virtual void updatePage(Buffer<uint_t> buffer, uint_t p) override;

    virtual Buffer<uint_t> readPage(uint_t p) override;

private:
    inline void deleteLRU();

    inline void updateAndMoveToFront(Buffer<uint_t> &buffer, uint_t p);

    inline void addNewCacheEntry(Buffer<uint_t> &buffer, uint_t p);

    inline bool cacheContains(uint_t p);

    inline bool cacheIsFull();

    inline Buffer<uint_t> moveToFront(uint_t p);
};

//
// Created by ThinkBook14G2 on 08-07-2022.
//


#include "LRUCacheStorage.h"

template<typename uint_t>
void LRUCacheStorage<uint_t>::appendPage(Buffer<uint_t> buffer) {
    int idBuf = Storage<uint_t>::getPages();
    Storage<uint_t>::appendPage(buffer);
    addNewCacheEntry(buffer, idBuf);
}

template<typename uint_t>
void LRUCacheStorage<uint_t>::updatePage(Buffer<uint_t> buffer, uint_t p) {
    buffer.setUpdated();
    if (cacheContains(p)) {
        updateAndMoveToFront(buffer, p);
    } else {
        addNewCacheEntry(buffer, p);
    }
}


template<typename uint_t>
Buffer<uint_t> LRUCacheStorage<uint_t>::readPage(uint_t p) {
    if (cacheContains(p)) {
        return moveToFront(p);
    } else {
        //ir a buscar página al disco
        Buffer<uint_t> out(Storage<uint_t>::getD());
        out = Storage<uint_t>::readPage(p);
        addNewCacheEntry(out, p);
        return out;
    }
}

template<typename uint_t>
inline Buffer<uint_t> LRUCacheStorage<uint_t>::moveToFront(uint_t p) {
    // assert(p is present in cache)
    // then move to front and return buffer.
    Buffer<uint_t> page(Storage<uint_t>::getD());
    page = *(map[p]);
    pageList.erase(map[p]);
    pageList.push_front(page);
    // update reference
    map[p] = pageList.begin();
    return page;
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::addNewCacheEntry(Buffer<uint_t> &buffer, uint_t p) {
    buffer.setId(p);
    if (cacheIsFull()) deleteLRU();
    pageList.push_front(buffer);
    map[p] = pageList.begin();
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::deleteLRU() {
    // delete least recently used element
    Buffer<uint_t> last(Storage<uint_t>::getD());
    last = pageList.back();
    // Pops the last element
    pageList.pop_back();
    // Erase the last
    map.erase(last.getId());
    //if changed, update on disk!
    if (last.isUpdated()) Storage<uint_t>::updatePage(last, last.getId());
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::updateAndMoveToFront(Buffer<uint_t> &buffer, uint_t p) {// present in cache
    buffer.setId(p);
    pageList.erase(map[p]);
    pageList.push_front(buffer);
    // update reference
    map[p] = pageList.begin();
}

template<typename uint_t>
inline bool LRUCacheStorage<uint_t>::cacheIsFull() {
    return this->pageList.size() == this->cacheSize;
}

template<typename uint_t>
inline bool LRUCacheStorage<uint_t>::cacheContains(uint_t p) { return map.find(p) != map.end(); }


#endif //PRUEBASC___LRUCACHESTORAGE_H
