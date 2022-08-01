//
// Created by Carlos on 01-08-2022.
//
#include "gtest/gtest.h"

TEST(prueba,malaIgualdad){
    EXPECT_EQ(9,2) << "Arroja el error correctamente";
    ASSERT_EQ(9,2) << "Continua correctamente";
    EXPECT_EQ(9,2) << "Este mensaje no deberia aparecer";
}
