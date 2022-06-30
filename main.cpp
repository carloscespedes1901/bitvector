#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Bitvector.h"


int main() {
    char buff[512];
    char buffTest[512];
    for(int i = 0; i < 256; i++){
        buff[i] = 'a';
        buffTest[i] = 'z';
    }
    Bitvector bitv("carlitos", 512);
    for(int i = 0; i < 20; i++){
        std::cout << "Appending (0 bien -1 mal. remplazar por assert): " << bitv.appendPage(buff) << std::endl;
        std::cout << "number of pages: " << bitv.getNumberOfPages() << std::endl;
    }
    for(int i = 0; i < 512; i++){
        buff[i] = 0;
    }
    std::cout << "contenido de mi buffer: " << buff << std::endl;
    std::cout << "Leyendo pagina 10 a buffer: " << bitv.readPage(buff, 10) << std::endl;
    std::cout << "contenido de mi buffer: " << buff << std::endl;
    for(int i = 0; i < 512; i++){
        buff[i] = 0;
    }
    std::cout << "contenido de mi buffer: " << buff << std::endl;
    std::cout << "escribiendo basura en la pagina 10: " << bitv.writePage(buffTest, 10) << std::endl;
    for(int i = 0; i < 512; i++){
        buff[i] = 0;
    }
    std::cout << "contenido de mi buffer: " << buff << std::endl;
    std::cout << "Leyendo pagina 10 a buffer: " << bitv.readPage(buff, 10) << std::endl;
    std::cout << "contenido de mi buffer: " << buff << std::endl;
    return 0;
}
