 set(BINARY ${Bitvector}_tst)

 file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

 set(SOURCES ${TEST_SOURCES})

 add_executable(${BINARY} ${TEST_SOURCES})

 add_test(<cambiar por nombre del test> ${BINARY} ¿COMMAND? ${BINARY})

 target_link_libraries(${BINARY} PUBLIC ${Bitvector}_lib gtest)