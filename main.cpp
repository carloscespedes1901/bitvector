#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Storage.h"


int main() {
    uint64_t buff[8];
    uint64_t buffTest[8];
    uint64_t readBuff[8]{0,0,0,0,0,0,0,0};
    for(int i = 0; i < 8; i++){
        buff[i] = i;
        buffTest[i] = i*i;
    }
    uint64_t a;
    Storage<uint64_t> bitv("testing.bin", 8);
    bitv.open("w");
    for(int i = 0; i < 20; i++){
        std::cout << "Appending (0 bien -1 mal. remplazar por assert): " << bitv.appendPage(buff) << std::endl;
        std::cout << "number of pages: " << bitv.getNumberOfPages() << std::endl;
    }
    bitv.close();
    bitv.open("rw");

    std::cout << "contenido de mi buffer guardado: " << buff << std::endl;
    std::cout << "Leyendo pagina 10 a buffer: " << bitv.readPage(readBuff, 10) << std::endl;
    std::cout << "contenido de mi buffer leido: " << readBuff << std::endl;

    std::cout << "contenido de mi buffer: " << buff << std::endl;
    std::cout << "escribiendo basura en la pagina 10: " << bitv.writePage(buffTest, 10) << std::endl;

    std::cout << "contenido de mi buffer update escritura: " << buffTest << std::endl;
    std::cout << "Leyendo pagina 10 a buffer: " << bitv.readPage(readBuff, 10) << std::endl;
    std::cout << "contenido de mi buffer: " << readBuff << std::endl;
    bitv.close();
    return 0;
}
