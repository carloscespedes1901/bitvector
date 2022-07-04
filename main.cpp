#include <iostream>
#include <cstdint>
#include "Storage.h"


void testPageByPage(){
    std::cout << "\n TEST LECTURA/ESCRITURA/APPEND DE A UNA PAGINA \n" << endl;
    uint64_t buff[8]{1};
    uint64_t buffTest[8]{1};
    uint64_t readBuff[8]{0};

    for(int i = 1; i < 8; i++){
        buff[i] = 2;
        buffTest[i] = 3;
    }

    Storage<uint64_t> bitv("PageByPage.bin", 8);
    bitv.open();

    for(int i = 0; i < 20; i++){
        std::cout << "Appending (0 bien -1 mal. remplazar por assert): " << bitv.appendPage(buff) << std::endl;
    }

    std::cout << "Leyendo pagina 10 a buffer: " << bitv.readPage(readBuff, 10) << std::endl;
    std::cout << "contenido de mi buffer leido: " << std::endl;

    for(int i=0; i < 8; i++){
        std::cout << "readBuff[" << i << "] = " << readBuff[i] << endl;
    }

    std::cout << "escribiendo basura en la pagina 10: " << bitv.writePage(buffTest, 10) << std::endl;
    std::cout << "Leyendo pagina 10 a buffer: " << bitv.readPage(readBuff, 10) << std::endl;
    std::cout << "contenido de mi buffer: " << std::endl;

    for(int i=0; i < 8; i++){
        std::cout << "readBuff[" << i << "] = " << readBuff[i] << endl;
    }

    bitv.close();
}

void testSeveralPages(){
    std::cout << "\n TEST LECTURA/ESCRITURA/APPEND DE A VARIAS PAGINAS \n" << endl;
    uint64_t buff[8*20];
    uint64_t buffTest[8*3];
    uint64_t readBuff[8*4]{0};

    for(int i = 0; i < 8*20; i++){
        if( i%8 == 0 ){
            buff[i] = 1;
        }else{
            buff[i] = 2;
        }
    }

    for(int i = 0; i < 8*3; i++){
        if( i%8 == 0 ){
            buffTest[i] = 1;
        }else{
            buffTest[i] = 3;
        }
    }

    Storage<uint64_t> bitv("SeveralPages.bin", 8);
    bitv.open();

    std::cout << "Appending 20 paginas: " << bitv.appendPages(buffTest,20) << std::endl;

    std::cout << "Leyendo paginas 10 a 13 a buffer: " << bitv.readPages(readBuff, 10, 4) << std::endl;
    std::cout << "contenido de mi buffer leido: " << std::endl;

    for(int i=0; i < 8*4; i++){
        std::cout << "readBuff[" << i << "] = " << readBuff[i] << endl;
    }

    std::cout << "escribiendo basura en las paginas 10 a 12: " << bitv.writePages(buffTest, 10, 3) << std::endl;
    std::cout << "Leyendo pagina 10 a 13 a buffer: " << bitv.readPages(readBuff, 10, 4) << std::endl;
    std::cout << "contenido de mi buffer: " << std::endl;

    for(int i=0; i < 8*4; i++){
        std::cout << "readBuff[" << i << "] = " << readBuff[i] << endl;
    }

    bitv.close();
}

int main() {
    testPageByPage();
    testSeveralPages();
    return 0;
}
