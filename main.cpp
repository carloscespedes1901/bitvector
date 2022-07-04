#include <iostream>
#include <stdint.h>
#include "Storage.h"


int main() {
    uint64_t buff[8];
    uint64_t buffTest[8];
    uint64_t readBuff[8]{0};

    for(int i = 1; i < 8; i++){
        buff[i] = 2;
        buffTest[i] = 3;
    }
    buff[0] = (buffTest[0] = 1);

    Storage<uint64_t> bitv("testing.bin", 8);
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

    /*
    std::cout << "Leyendo todo el bitvector: " << std::endl;

    for(int i = 0; i < 20; i++){
        memset(readBuff, 0, 8*sizeof(uint64_t));
        std::cout << "Leyendo pagina " << i << " a buffer: " << bitv.readPage(readBuff, i) << std::endl;
        std::cout << "contenido de mi buffer: " << std::endl;

        for(int i=0; i < 8; i++){
            std::cout << "readBuff[" << i << "] = " << readBuff[i] << endl;
        }
    }
    */

    bitv.close();
    return 0;
}
