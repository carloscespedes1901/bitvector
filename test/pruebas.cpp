//
// Created by Carlos on 01-08-2022.
//
#include "gtest/gtest.h"
#include "Storage.h"
#include "LRUCacheStorage.h"
#include "ExternalBitvect.h"

TEST(Expect64Test, Expect64){
    ASSERT_EQ(8,sizeof(uint64_t)) << "No se reconoce uint64_t como entero de 64 bit";
}

TEST(BitvectorTest, testStorage){ // ¿Cambiar por fixture class?
    /* ------------------------- */
    Buffer<uint64_t> buffer(10);
    buffer.createBlock();
    Buffer<uint64_t> bufferRead(10);
    bufferRead.createBlock(true);
    /* ------------------------- */

    //LLenamos un buffer con basura
    for (int i = 0; i < 10; i++)
        buffer[i] = UINT32_MAX;

    Storage<uint64_t> pruebaStorage("pruebaStorage.bin", 10);

    ASSERT_TRUE(pruebaStorage.create()); //lo crea solo sino existe;
    ASSERT_TRUE(pruebaStorage.open());

    // LLenamos 10 paginas con basura
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],buffer[i]);
}

TEST(BitvectorTest,testExternalBitVector){
    Storage<uint64_t> vectorStorage("Testing.bin", 10);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);
    pruebaVector.create(1000);
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);

    EXPECT_EQ(pruebaVector.access(512),1);
    EXPECT_EQ(pruebaVector.access(513),1);
    EXPECT_EQ(pruebaVector.access(514),1);
    EXPECT_EQ(pruebaVector.access(515),1);

    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);

    EXPECT_EQ(pruebaVector.access(512),0);
    EXPECT_EQ(pruebaVector.access(513),0);
    EXPECT_EQ(pruebaVector.access(514),0);
    EXPECT_EQ(pruebaVector.access(515),0);

    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4) {
        pruebaVector.bitSet(i);
    }

    for (uint64_t i = 0; i < pruebaVector.getLength(); i ++) {
        if(i%4==0)
            EXPECT_EQ(pruebaVector.access(i),1);
        else
            EXPECT_EQ(pruebaVector.access(i),0);
    }

    pruebaVector.buildRank();
    EXPECT_EQ(pruebaVector.rank((uint64_t)999),250);
}

TEST(BitvectorTest,testBits) {
    uint64_t bits = 0, i = 0;
    int bit;
    EXPECT_EQ(BitBasic::access(&bits, (uint64_t) 0, 64),0);
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    EXPECT_EQ(BitBasic::access(&bits, (uint64_t) 0, 64),1);
    for (uint64_t j = 0; j < 64; j++) {
        bit = (BitBasic::access(&bits, j, 64) ? 1 : 0);
        if(j == 0)
            EXPECT_EQ(bit,1);
        else
            EXPECT_EQ(bit,0);
    }
    // 8000 hexadecimal = 1000000000000000000000000000000000000000000000000000000000000000 binario
    EXPECT_EQ(bitset<64>(bits),0x8000000000000000);

    i = 2;
    EXPECT_EQ(BitBasic::access(&bits, i, 64),0);
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    EXPECT_EQ(BitBasic::access(&bits, i, 64),1);

    for (uint64_t j = 0; j < 64; j++) {
        bit = (BitBasic::access(&bits, j, 64) ? 1 : 0);
        if(j == 0 || j == 2)
            EXPECT_EQ(bit,1);
        else
            EXPECT_EQ(bit,0);
    }

    EXPECT_EQ(bitset<64>(bits),0xA000000000000000);

    i = 4;

    EXPECT_EQ(BitBasic::access(&bits, i, 64),0);
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    EXPECT_EQ(BitBasic::access(&bits, i, 64),1);

    for (uint64_t j = 0; j < 64; j++) {
        bit = (BitBasic::access(&bits, j, 64) ? 1 : 0);
        if(j == 0 || j == 2 || j == 4)
            EXPECT_EQ(bit,1);
        else
            EXPECT_EQ(bit,0);
    }
    EXPECT_EQ(bitset<64>(bits),0xA800000000000000);

    BitBasic::BitClear(&bits, (uint64_t) 0, 64);
    BitBasic::BitClear(&bits, (uint64_t) 2, 64);
    BitBasic::BitClear(&bits, (uint64_t) 4, 64);

    for (int j = 0; j < 64; j++) {
        EXPECT_EQ(BitBasic::access<uint64_t>(&bits, j, 64),0);
    }

    EXPECT_EQ(bitset<64>(bits),0);

    BitBasic::BitSet(&bits, (uint64_t) 10, 64);
    BitBasic::BitSet(&bits, (uint64_t) 11, 64);
    BitBasic::BitSet(&bits, (uint64_t) 12, 64);
    BitBasic::BitSet(&bits, (uint64_t) 13, 64);

    EXPECT_EQ(bitset<64>(bits),0x003C000000000000);

    for (int j = 0; j < 64; j++) {
        if(j>=10 && j<=13)
            EXPECT_EQ(BitBasic::access<uint64_t>(&bits, j, 64),1);
        else
            EXPECT_EQ(BitBasic::access<uint64_t>(&bits, j, 64),0);
    }

    i = 13;
    EXPECT_EQ(BitBasic::Rank(&bits, i, 64),4);
}

TEST(BitvectorTest,testExternalBitVectorWithLRU) {
    LRUCacheStorage<uint64_t> vectorStorage("Testing.bin", 10,20);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);

    pruebaVector.create(1000);

    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);

    EXPECT_EQ(pruebaVector.access(512),1);
    EXPECT_EQ(pruebaVector.access(513),1);
    EXPECT_EQ(pruebaVector.access(514),1);
    EXPECT_EQ(pruebaVector.access(515),1);

    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);

    EXPECT_EQ(pruebaVector.access(512),0);
    EXPECT_EQ(pruebaVector.access(513),0);
    EXPECT_EQ(pruebaVector.access(514),0);
    EXPECT_EQ(pruebaVector.access(515),0);

    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4) {
        pruebaVector.bitSet(i);
    }

    uint64_t pos = 0;
    for (uint64_t i = 0; i < pruebaVector.getLength(); i ++) {
            if(i%4==0)
        EXPECT_EQ(pruebaVector.access(i),1);
            else
        EXPECT_EQ(pruebaVector.access(i),0);
    }

    pruebaVector.buildRank();
    EXPECT_EQ(pruebaVector.rank((uint64_t)999),250);
}