//
// Created by ThinkBook14G2 on 04-08-2022.
// Least Frequently Used (LFU) Cache Implementation
// reference: https://www.enjoyalgorithms.com/blog/least-frequently-used-cache
//

#ifndef PRUEBASC___LFUCACHESTORAGE_H
#define PRUEBASC___LFUCACHESTORAGE_H

#include <list>
#include <unordered_map>
#include "Storage.h"

template<typename uint_t>
class LFUCacheStorage : public Storage<uint_t> {
private:
    // store page of cache
    map<uint_t, list<Page<uint_t>>> mapPageList;
    // store references of page in pageList of key in cache
    unordered_map<uint_t, typename list<Page<uint_t>>::iterator> map;
    int cacheSize; // maximum capacity of cache

public:
    LFUCacheStorage(const string &fileName, int d, int capacity) : Storage<uint_t>(fileName, d), cacheSize(capacity) {}

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

template<typename uint_t>
void LFUCacheStorage<uint_t>::appendPage(Page<uint_t> buffer) {
    int idBuf = Storage<uint_t>::getPages();
    Storage<uint_t>::appendPage(buffer);
    addNewCacheEntry(buffer, idBuf);
}

template<typename uint_t>
void LFUCacheStorage<uint_t>::updatePage(Page<uint_t> buffer, uint_t p) {
    buffer.setUpdated();
    if (cacheContains(p)) {
        updateCacheEntry(buffer, p);
    } else {
        addNewCacheEntry(buffer, p);
    }
}


template<typename uint_t>
Page<uint_t> LFUCacheStorage<uint_t>::readPage(uint_t p) {
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
void LFUCacheStorage<uint_t>::close() {
    //save the cache changed block
    for(auto &mapEntry: mapPageList) {
        for (Page<uint_t> &page: mapEntry.second) {
            if (page.isUpdated()) Storage<uint_t>::updatePage(page, page.getId());
        }
        mapEntry.second.clear();
    }
    mapPageList.clear();
    map.clear();
    Storage<uint_t>::close();
}
//Police for LFU cache
template<typename uint_t>
inline Page<uint_t> LFUCacheStorage<uint_t>::getCacheEntry(uint_t p) {
    // assert(p is present in cache)
    // then move to the next list of frecuency and return buffer.
    Page<uint_t> page(Storage<uint_t>::getD());
    page = *(map[p]);
    mapPageList[page.getFrec()].erase(map[p]);
    page.increaceFrec();
    mapPageList[page.getFrec()].push_front(page);
    // update reference
    map[p] = mapPageList[page.getFrec()].begin();
    return page;
}

template<typename uint_t>
inline void LFUCacheStorage<uint_t>::addNewCacheEntry(Page<uint_t> &buffer, uint_t p) {
    if (cacheIsFull()) removeCacheEntry();
    buffer.startCountFrec();
    mapPageList[buffer.getFrec()].push_front(buffer);
    map[p] = mapPageList[buffer.getFrec()].begin();
}

template<typename uint_t>
inline void LFUCacheStorage<uint_t>::removeCacheEntry() {
    // delete least frecuently used. If exists more tan one, remove least recently used element from this list
    Page<uint_t> last(Storage<uint_t>::getD());
    auto &LFU_List=mapPageList.begin()->second;
    last =  LFU_List.back();
    // Pops the last element
    LFU_List.pop_back();
    // Erase the last
    map.erase(last.getId());
    //if changed, update on disk!
    if (last.isUpdated()) Storage<uint_t>::updatePage(last, last.getId());
}

template<typename uint_t>
inline void LFUCacheStorage<uint_t>::updateCacheEntry(Page<uint_t> &buffer, uint_t p) {// present in cache
    //buffer is previously gets from cache. Page isn't a new entry.
    mapPageList[buffer.getFrec()].erase(map[p]);
    buffer.increaceFrec();
    mapPageList[buffer.getFrec()].push_front(buffer);
    // update reference
    map[p] =  mapPageList[buffer.getFrec()].begin();

}

template<typename uint_t>
inline bool LFUCacheStorage<uint_t>::cacheIsFull() {
    return this->map.size() == this->cacheSize;
}

template<typename uint_t>
inline bool LFUCacheStorage<uint_t>::cacheContains(uint_t p) { return map.find(p) != map.end(); }

#endif //PRUEBASC___LFUCACHESTORAGE_H
