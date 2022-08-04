//
// Created by ThinkBook14G2 on 08-07-2022.
//
//based on //from: https://github.com/fclaude/libcds2/blob/master/include/libcds2/libcds.h

#ifndef PRUEBASC___BITBASIC_H
#define PRUEBASC___BITBASIC_H

#include <bit>
#include <bits/stdc++.h>

using namespace std;
namespace BitBasic {
//indexa los bits de izquierda a derecha hasta el infinito...
    template<typename uint_t>
    inline bool access(const uint_t *e, const uint_t p, const int W) {
        return 0 != ((e[p / W] & (uint_t) 1 << (W - 1 - (p % W))));
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
        uint_t sum = 0;
        for (auto i = 0; i < (p / W); i++) {
            sum += popcount(e[i]);
        }
        sum += popcount(e[p / W] >> (W - 1 - (p % W)));
        return sum;
    }

    uint64_t MASK_11 = 0b11;

    inline uint64_t getGroup(uint64_t x, int height) {
        return x >> (2 * height + 2);
    }

    inline uint8_t getChild(uint64_t x, int height) {
        return (uint8_t) ((x >> (2 * height)) & MASK_11);
    }
//A array, p position, chunk is bitarray to set
//      i=       0         1         2         3
//ejemplo: [0000,0000|0000,0000|0000,0000|0000,0000]
//    P=     0    1     2   3    4    5    6    7
//set chunk=[0010], in p=3
//     i=       0         1         2         3
//         [0000,0000|0000,0000|0000,0000|0000,0000]
//   p=      0    1     2   3    4    5    6    7
//               OR [0000,0010]
//set chunk=[0010], in p=4
//     i=       0         1         2         3
//         [0000,0000|0000,0000|0000,0000|0000,0000]
//     p=    0    1     2   3    4    5    6    7
//                         OR [0010,0000]

    inline void setChunk(uint8_t *A, uint32_t p, uint8_t chunk) {
        int i = p / 2;
        uint8_t byteOri = A[i];
        if ((p & 1) == 0) {//0 (even) is hi bits position chunk
            A[i] |= (chunk << 4);
        } else {
            A[i] |= chunk;
        }
        cout << "Guardando Chunk " << (p+1) <<"° (" << bitset<4>(chunk)
             << ") en el byte " << i << " del buffer. "
             << " valor antes: " << bitset<8>(byteOri)
             << " valor ahora: " << bitset<8>(A[i]) << endl;
    }


//z-order, from web:http://www-graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
// Interleave lower 16 bits of x and y, so the bits of x
// are in the even positions and bits from y in the odd;
// x and y must initially be less than 65536.
//return z, z gets the resulting 32-bit Morton Number.

    inline uint32_t zorder(uint32_t x, uint32_t y) {
        const static uint32_t B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
        const static uint32_t S[] = {1, 2, 4, 8};

        uint32_t z = 0; // z gets the resulting 32-bit Morton Number.
        // x and y must initially be less than 65536.

        x = (x | (x << S[3])) & B[3];
        x = (x | (x << S[2])) & B[2];
        x = (x | (x << S[1])) & B[1];
        x = (x | (x << S[0])) & B[0];

        y = (y | (y << S[3])) & B[3];
        y = (y | (y << S[2])) & B[2];
        y = (y | (y << S[1])) & B[1];
        y = (y | (y << S[0])) & B[0];

        z = x | (y << 1);

        return z;
    }


    uint64_t zorder64(uint32_t x, uint32_t y) {
        uint64_t z_low = zorder(x, y);
        uint64_t z_hi = zorder(x >> 16, y >> 16);
        return (z_hi << 32) | z_low;
    }

    //retorna 2 uint64_t
    //out[0] es low, out[1] es hi.
    vector<uint64_t> zorder128(uint64_t x, uint64_t y) {
        vector<uint64_t> out;
        uint32_t _x = (uint32_t) x;
        uint32_t _y = (uint32_t) y;
        out[0] = zorder64(_x, _y);
        _x = (uint32_t) (x >> 32);
        _y = (uint32_t) (y >> 32);
        out[1] = zorder64(_x, _y);
        return out;
    }

    //se invierte el orden del par (x,y) en los parámetros, para hacerlo compatible con el model
    //del k2tree de (fila, columna), dado que x es la columna y la y es la fila
    vector<uint64_t> generateZOrder(vector<uint32_t> &Y, vector<uint32_t> &X) {
        vector<uint64_t> output;
        for (size_t i = 0; i < X.size(); i++) {
            output.push_back(zorder64(X[i], Y[i]));
        }
        return output;
    }
}
#endif