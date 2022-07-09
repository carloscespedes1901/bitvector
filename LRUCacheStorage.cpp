//
// Created by ThinkBook14G2 on 08-07-2022.
//

#include "LRUCacheStorage.h"

template<typename uint_t>
void LRUCacheStorage<uint_t>::appendPage(const uint_t *buffer) {
    int id=Storage<uint_t>::getPages();
    uint_t *page;
    bool isChanged;
    Storage<uint_t>::appendPage(buffer);

    if (pageList.size() == csize) {
        // delete least recently used element
        tie(id, page,isChanged) = pageList.back();
        // Pops the last element
        pageList.pop_back();
        // Erase the last
        map.erase(id);
        if(isChanged) Storage<uint_t>::updatePage(page, id);
        delete page;
    }
    pageList.push_front(make_tuple(id, buffer, false));
    map[id] = pageList.begin();


}

template<typename uint_t>
void LRUCacheStorage<uint_t>::updatePage(const uint_t *buffer, uint_t p) {
    int id;
    uint_t *page;
    bool isChanged;
    // not present in cache
    if (map.find(p) == map.end()) {
        // cache is full
        if (pageList.size() == csize) {
            // delete least recently used element
            tie(id, page,isChanged) = pageList.back();
            // Pops the last element
            pageList.pop_back();
            // Erase the last
            map.erase(id);
            //la actualización de las páginas ocurren cuando salen del caché solamente.
            if(isChanged) Storage<uint_t>::updatePage(page, id);
            delete page;
        }
        //actualizar el caché
        pageList.push_front(make_tuple(p, buffer, true));
        map[p] = pageList.begin();
    } else {
        // present in cache
        tie(id, page) = (*map[p]);
        pageList.erase(map[p]);
        pageList.push_front(make_tuple(p, buffer, false));
        // update reference
        map[p] = pageList.begin();
        if(buffer!=page){
            delete(page);
        }
    }

}

template<typename uint_t>
void LRUCacheStorage<uint_t>::readPage(uint_t *&buffer, uint_t p) {
    int id;
    uint_t *page;
    bool isChanged;
    // not present in cache
    if (map.find(p) == map.end()) {
        // cache is full
        if (pageList.size() == csize) {
            // delete least recently used element
            tie(id, page,isChanged) = pageList.back();
            // Pops the last element
            pageList.pop_back();
            // Erase the last
            map.erase(id);
            if(isChanged) Storage<uint_t>::updatePage(page, id);
            delete page;
        }
        //page = new uint_t[this->getD()];
        //ir a buscar página al disco
        Storage<uint_t>::readPage(buffer, p);
        pageList.push_front(make_tuple(p, buffer, false));
        map[p] = pageList.begin();
    } else {
        // present in cache
        tie(id, page) = (*map[p]);
        pageList.erase(map[p]);
        pageList.push_front(make_tuple(p, page, false));
        // update reference
        map[p] = pageList.begin();
        delete(buffer);
        buffer=page;
    }
}

