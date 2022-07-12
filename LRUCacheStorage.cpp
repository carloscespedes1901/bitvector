//
// Created by ThinkBook14G2 on 08-07-2022.
//


#include "LRUCacheStorage.h"

template<typename uint_t>
void LRUCacheStorage<uint_t>::appendPage(const Buffer<uint_t> buffer) {
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
        out.createBlock();
        out = Storage<uint_t>::readPage(p, 0);
        addNewCacheEntry(out, p);
        return out;
    }
}

template<typename uint_t>
inline Buffer<uint_t> LRUCacheStorage<uint_t>::moveToFront(uint_t p) const {
    // assert(p is present in cache)
    // then move to front and return buffer.
    Buffer<uint_t> page(Storage<uint_t>::getD());
    int id;
    auto iter = map[p];


    tie(id, page) = (*iter);
    pageList.erase(iter);
    pageList.push_front(make_tuple(p, page));
    // update reference
    map[p] = pageList.begin();
    return page;
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::addNewCacheEntry(const Buffer<uint_t> &buffer, uint_t p) {
    if (cacheIsFull()) deleteLRU();
    pageList.push_front(make_tuple(p, buffer));
    map[p] = pageList.begin();
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::deleteLRU() {
    // delete least recently used element
    int id_last;
    Buffer<uint_t> last(Storage<uint_t>::getD());
    tie(id_last, last) = pageList.back();
    // Pops the last element
    pageList.pop_back();
    // Erase the last
    map.erase(id_last);
    //if changed, update on disk!
    if (last.isUpdated()) Storage<uint_t>::updatePage(last, id_last);
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::updateAndMoveToFront(const Buffer<uint_t> &buffer, uint_t p) {// present in cache
    pageList.erase(map[p]);
    pageList.push_front(make_tuple(p, buffer));
    // update reference
    map[p] = pageList.begin();
}

template<typename uint_t>
inline bool LRUCacheStorage<uint_t>::cacheIsFull() {
    return this->pageList.size() == this->cacheSize;
}

template<typename uint_t>
inline bool LRUCacheStorage<uint_t>::cacheContains(uint_t p) { return map.find(p) != map.end(); }
