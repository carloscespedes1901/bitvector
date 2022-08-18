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
    list <Page<uint_t>> pageList;
    // store references of page in pageList of key in cache
    unordered_map<uint_t, typename list<Page<uint_t>>::iterator> map;
    int cacheSize; // maximum capacity of cache

public:
    LRUCacheStorage(const string &fileName, int d, int csize) : Storage<uint_t>(fileName, d), cacheSize(csize) {}

    virtual void appendPage(Page<uint_t> buffer) override;

    virtual void updatePage(Page<uint_t> buffer, uint_t p) override;

    virtual Page<uint_t> readPage(uint_t p) override;

    void close() override;

private:
    inline void removeCacheEntry();

    inline Page<uint_t> getCacheEntry(uint_t p);

    inline void updateCacheEntry(Page<uint_t> &buffer, uint_t p);

    inline void addNewCacheEntry(Page<uint_t> &buffer, uint_t p);

    inline bool cacheContains(uint_t p);

    inline bool cacheIsFull();
};

//
// Created by ThinkBook14G2 on 08-07-2022.
//


#include "LRUCacheStorage.h"

template<typename uint_t>
void LRUCacheStorage<uint_t>::appendPage(Page<uint_t> buffer) {
    int idBuf = Storage<uint_t>::getPages();
    Storage<uint_t>::appendPage(buffer);
    addNewCacheEntry(buffer, idBuf);
}

template<typename uint_t>
void LRUCacheStorage<uint_t>::updatePage(Page<uint_t> buffer, uint_t p) {
    buffer.setUpdated();
    if (cacheContains(p)) {
        updateCacheEntry(buffer, p);
    } else {
        addNewCacheEntry(buffer, p);
    }
}


template<typename uint_t>
Page<uint_t> LRUCacheStorage<uint_t>::readPage(uint_t p) {
    if (cacheContains(p)) {
        return getCacheEntry(p);
    } else {
        //ir a buscar página al disco
        Page<uint_t> out(Storage<uint_t>::getD());
        out = Storage<uint_t>::readPage(p);
        addNewCacheEntry(out, p);
        return out;
    }
}

template<typename uint_t>
inline Page<uint_t> LRUCacheStorage<uint_t>::getCacheEntry(uint_t p) {
    // assert(p is present in cache)
    // then move to front and return buffer.
    Page<uint_t> page(Storage<uint_t>::getD());
    page = *(map[p]);
    pageList.erase(map[p]);
    pageList.push_front(page);
    // update reference
    map[p] = pageList.begin();
    return page;
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::addNewCacheEntry(Page<uint_t> &buffer, uint_t p) {
    if (cacheIsFull()) removeCacheEntry();
    pageList.push_front(buffer);
    map[p] = pageList.begin();
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::removeCacheEntry() {
    // delete least recently used element
    Page<uint_t> last(Storage<uint_t>::getD());
    last = pageList.back();
    // Pops the last element
    pageList.pop_back();
    // Erase the last
    map.erase(last.getId());
    //if changed, update on disk!
    if (last.isUpdated()) Storage<uint_t>::updatePage(last, last.getId());
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::updateCacheEntry(Page<uint_t> &buffer, uint_t p) {// present in cache
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

template<typename uint_t>
void LRUCacheStorage<uint_t>::close() {
    //save the cache changed block
    for (Page<uint_t> &page: pageList) {
        if (page.isUpdated()) Storage<uint_t>::updatePage(page, page.getId());
    }
    pageList.clear();
    map.clear();
    Storage<uint_t>::close();
}


#endif //PRUEBASC___LRUCACHESTORAGE_H
