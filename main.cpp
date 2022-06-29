#include <iostream>
#include "cmake-build-debug\pages.h"


int main() {
    std::cout << "Crear directorios de prueba: " << std::endl;
    createFolders("MiPrimerBitvector");
    return 0;
}
