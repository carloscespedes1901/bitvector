//
// Created by ThinkBook14G2 on 08-07-2022.
//
//based on //from: https://github.com/fclaude/libcds2/blob/master/include/libcds2/libcds.h

#ifndef PRUEBASC___BITBASIC_H
#define PRUEBASC___BITBASIC_H
#include <bits/stdc++.h>
#include <bit>
using namespace std;
namespace BitBasic {
//indexa los bits de izquierda a derecha hasta el infinito...
    template<typename uint_t>
    inline bool access(const uint_t *e, const uint_t p, const int W) {
        return 0 != ((e[p / W] & (uint_t)1 << (W - 1 - (p % W))));
    }

    template<typename uint_t>
    inline void BitSet(uint_t *e, const uint_t p, const int W) {
        e[p / W] |= ((uint_t) 1 << (W - 1 - (p % W)));
    }

    template<typename uint_t>
    inline void BitClear(uint_t *e, const uint_t p, const int W) {
        e[p / W] &= ~((uint_t) 1 << (W - 1 - (p % W)));
    }

    void printBitArray(const uint8_t *T, size_t T_size) {
        for (int i = 0; i < T_size; i++) {
            cout << access<uint8_t>(T, i, 8);
            if ((i + 1) % 4 == 0) {
                cout << " ";
            } else if ((i + 1) % 64 == 0) {
                cout << endl;
            }
        }
    }

    template<typename uint_t>
    inline uint_t Rank(uint_t *e, const uint_t p, const int W) {
        uint_t sum=0;
        for(auto i=0;i<(p/W);i++){
            sum+=popcount(e[i]);
        }
        sum+=popcount(e[p/W]>>(W - 1 - (p % W)));
        return sum;
    }
}
#endif