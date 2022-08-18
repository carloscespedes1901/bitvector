//
// Created by ThinkBook14G2 on 13-07-2022.
//

#ifndef PRUEBASC___EXTERNALK2TREE_H
#define PRUEBASC___EXTERNALK2TREE_H

#include "ExternalBitvect.h"
#include "BitBasic.h"
#include <vector>

using namespace std;
using namespace BitBasic;

template<typename uint_t>
class ExternalK2Tree {
private:
    //unsigned long long, para poder leer de forma segura.
    unsigned long long K = 2;
    unsigned long long nodes = 0;
    unsigned long long edges = 0;
    unsigned long long matrix_size = 0;
    ExternalBitvector<uint_t> T_L;
    Storage<uint_t> &storage;
public:
    //constructores
    ExternalK2Tree(uint_t k, Storage<uint_t> &storage) : K(k), storage(storage),
                                                         T_L(ExternalBitvector<uint_t>(storage)) {
        storage.setMeta("K2tree_K", to_string(this->K));
        storage.setMeta("K2tree_NODES", to_string(nodes));
        storage.setMeta("K2tree_EDGES", to_string(edges));
        storage.setMeta("K2Tree_MATRIX_SIZE", to_string(matrix_size));
    }

    virtual ~ExternalK2Tree();

    void create(const char *pointFilename);

    void open() {
        T_L.open();
        K = stoull(storage.getMeta("K2tree_K"));
        nodes = stoull(storage.getMeta("K2tree_NODES"));
        edges = stoull(storage.getMeta("K2tree_EDGES"));
        matrix_size = stoull(storage.getMeta("K2Tree_MATRIX_SIZE"));
    }

    //geters
    inline uint_t getK() const {
        return K;
    }

    inline uint_t getNodes() const {
        return nodes;
    }

    inline uint_t getEdges() const {
        return edges;
    }
    //desde archivo con puntos


    /**
       *
       * método checkLink,
       * permite conocer si una celda (i,j) está en 1 (verdadero) o en 0 (false).
       *
       * @param i,    fila de la celda consultada
       * @param j,    columna de la celda consultada
       * @return, verdadero si la celda (i,j) está en 1.
   */
    inline bool checkLink(uint_t i, uint_t j) {
        return checkLink(i, j, -1, nodes);
    }

    inline vector<uint_t> direct(uint_t row) {
        vector<uint_t> neighbors;
        directNeighbors(nodes, row, 0, -1, neighbors);
        return neighbors;
    }

    inline vector<uint_t> reverse(uint_t column) {
        vector<uint_t> neighbors;
        reverseNeighbors(nodes, column, 0, -1, neighbors);
        return neighbors;
    }
    //vecinos directos
    //vecinos reversos
    //consulta por rango
    /**
     * Permite obtener todos los puntos (x,y), tal que qx1 <= x <= qx2 y qy1 <= y <= qy2
     *
     * @param qx1 límite inferior del rango en filas
     * @param qx2 límite superior del rango en filas
     * @param qy1 límite inferior del rango en columnas
     * @param qy2 límite superior del rango en columnas
     * @return
     */
    vector<pair<uint_t, uint_t>> range(uint_t qx1, uint_t qx2, uint_t qy1, uint_t qy2) {
        vector<pair<uint_t, uint_t>> result;
        range(qx1, qx2, qy1, qy2, -1, 0, 0, matrix_size, result);
        return result;
    }

    void close();

private:
    /**
        * Método interno que resuelve la consulta de checkLink. Basado en el algoritmo 12.4 página 199 de la tesis
        * de Susana Ladra.
        * @param i fila consultada
        * @param j columna consultada
        * @param node nodo que actualmente se visita
        * @param ancho ancho del cuadrante asociado al nodo visitado
        * @return
        */
    bool checkLink(uint_t i, uint_t j, uint_t node, uint_t ancho);

    void directNeighbors(uint_t subMatrixSize, uint_t row, uint columnOffset, uint_t node, vector<uint_t> &neighbors);

    void
    reverseNeighbors(uint_t subMatrixSize, uint_t column, uint rowOffset, uint_t node, vector<uint_t> &neighbors);
    /**
     * Método interno que resuelve la consulta de los vecinos directos. Basado en
     */
    /**
    * Método interno que resuelve las consultas por rango. Basado en el algoritmo 12.5 página 200 tésis
    * de Susana Ladra.
    * Entrada:
    * @param qx1 límite inferior del rango de las filas consultadas
    * @param qx2 límite superior del rango de las filas  consultadas
    * @param qy1 límite inferior del rango de las columnas consultadas
    * @param qy2 límite inferior del rango de las columnas consultadas.
    * @param node posición en TL del nodo visitado
    * @param x Coordenada x de la celda superior izquierda del cuadrante asociado al nodo visitado
    * @param y Coordenada y de la celda superior izauierda del cuadrante asociado al nodo visitado
    * @param ancho ancho del cuadrante asociado al nodo visitado
    *
    * Salida:
    * @param result, ArrayList que permite acumular los pares (x,y) que representan la fila (x) y la columna (y)
    *                de una celda en 1 dentro del rango de la consulta.
    */
    void
    range(uint_t qx1, uint_t qx2, uint_t qy1, uint_t qy2, uint_t node, uint_t x, uint_t y, uint_t ancho,
          vector<pair<uint_t, uint_t>> &result);


    inline uint_t hermano(uint_t ph, uint_t x, uint_t y) {
        uint_t posHijo = x * K + y;
        return ph + posHijo;
    }

    inline uint_t primerHijo(uint_t node) {
        return (node == (uint_t) -1) ? 0 : T_L.rank(node) * K * K;
    }

};


template<typename uint_t>
bool ExternalK2Tree<uint_t>::checkLink(uint_t i, uint_t j, uint_t node, uint_t ancho) {
    if (ancho == 1) {
        return T_L.access(node);
    } else {
        uint_t anchoHijo = ancho / K;
        uint_t h = hermano(primerHijo(node), i / anchoHijo, j / anchoHijo);
        if (T_L.access(h) == 1) {
            return checkLink(i % anchoHijo, j % anchoHijo, h, anchoHijo);
        }
        return false;
    }
}

template<typename uint_t>
void ExternalK2Tree<uint_t>::directNeighbors(uint_t matrixSize, uint_t row, uint columnOffset, uint_t node,
                                             vector<uint_t> &neighbors) {
    //cout << "direct(" << matrixSize << ", " << row << ", " << columnOffset << ", " << node << ")" << endl;

    if (matrixSize == 1) {
        if (T_L.access(node) == 1) {
            neighbors.push_back(columnOffset);
        }
    } else {
        uint_t subMatrixSize = matrixSize / K;
        uint_t phRow = primerHijo(node) + K * (row / subMatrixSize);
        uint_t hijo;
        for (int j = 0; j < K; j++) {
            hijo = phRow + j;
            if (T_L.access(hijo) == 1) {
                directNeighbors(subMatrixSize, row % subMatrixSize,
                                columnOffset + subMatrixSize * j, hijo, neighbors);
            }
        }
    }
}

template<typename uint_t>
void ExternalK2Tree<uint_t>::reverseNeighbors(uint_t matrixSize, uint_t column, uint rowOffset, uint_t node,
                                              vector<uint_t> &neighbors) {
    if (matrixSize == 1) {
        if (T_L.access(node)) {
            neighbors.push_back(rowOffset);
        }
        return;
    } else {
        uint_t subMatrixSize = matrixSize / K;
        uint_t phCol = primerHijo(node) + (column / subMatrixSize);
        uint_t hijo;
        for (int i = 0; i < K; i++) {
            hijo = phCol + i * K;
            if (T_L.access(hijo) == 1) {
                reverseNeighbors(subMatrixSize, column % subMatrixSize,
                                rowOffset + subMatrixSize * i, hijo, neighbors);
            }
        }
    }
}


/**
    * Método interno que resuelve las consultas por rango. Basado en el algoritmo 12.5 página 200 tésis
    * de Susana Ladra.
    * Entrada:
    * @param qx1 límite inferior del rango de las filas consultadas
    * @param qx2 límite superior del rango de las filas  consultadas
    * @param qy1 límite inferior del rango de las columnas consultadas
    * @param qy2 límite inferior del rango de las columnas consultadas.
    * @param node posición en TL del nodo visitado
    * @param x Coordenada x de la celda superior izquierda del cuadrante asociado al nodo visitado
    * @param y Coordenada y de la celda superior izauierda del cuadrante asociado al nodo visitado
    * @param ancho ancho del cuadrante asociado al nodo visitado
    *
    * Salida:
    * @param result, ArrayList que permite acumular los pares (x,y) que representan la fila (x) y la columna (y)
    *                de una celda en 1 dentro del rango de la consulta.
    */
template<typename uint_t>
void ExternalK2Tree<uint_t>::range(uint_t qx1, uint_t qx2, uint_t qy1, uint_t qy2, uint_t node, uint_t x, uint_t y,
                                   uint_t ancho,
                                   vector<pair<uint_t, uint_t>> &result) {
    if (ancho == 1) {
        //si es hoja de último nivel
        result.push_back(make_pair(x, y));
    } else {
        uint_t anchoHijo = ancho / K;
        uint_t ph = primerHijo(node);
        for (uint_t i = (qx1 / anchoHijo); i <= (qx2 / anchoHijo); i++) {
            //ajusto el rango en x de la consulta al espacio del hijo para
            //la llamada recursiva.
            uint_t x1 = (i == (qx1 / anchoHijo)) ? qx1 % anchoHijo : 0;
            uint_t x2 = (i == (qx2 / anchoHijo)) ? qx2 % anchoHijo : anchoHijo - 1;
            for (uint_t j = (qy1 / anchoHijo); j <= (qy2 / anchoHijo); j++) {
                //ajusto el rango en y de la consulta para la llamada recursiva.
                //obtengo el nodo que corresponde
                uint_t y1 = (i == (qx1 / anchoHijo)) ? qy1 % anchoHijo : 0;
                uint_t y2 = (i == (qx2 / anchoHijo)) ? qy2 % anchoHijo : anchoHijo - 1;
                uint_t h = hermano(ph, i, j);
                if (T_L.access(h) == 1) {
                    range(x1, x2, y1, y2, h, x + anchoHijo * i, y + anchoHijo * j, anchoHijo, result);
                }
            }
        }
    }
}


void read(const char *fileName, uint32_t &max, vector<uint32_t> &X, vector<uint32_t> &Y) {
    ifstream input;

    input.open(fileName);
    if (!input.is_open()) {
        cerr << "No se pudo abrir el archivo: " << fileName << std::endl;
        return;
    }
    uint32_t a, b;
    max = 0;
    while (input >> a >> b) {
        if (a > max) {
            max = a;
        }
        if (b > max) {
            max = b;
        }
        X.push_back(a);
        Y.push_back(b);

    }
    input.close();
}

//create k2tree with k=2
template<typename uint_t>
void ExternalK2Tree<uint_t>::create(const char *point_fileName) {
    //leer los puntos
    //generar k2tree
    //guardarlo en el formato usado
    T_L.create();//lo crea y lo deja abierto

    uint32_t max = 0;
    //1°) lee los puntos (x,y) y convierte a Z
    vector<uint32_t> X;
    vector<uint32_t> Y;
    read(point_fileName, max, X, Y);
    vector<uint64_t> zPoint = generateZOrder(X, Y);
    const uint8_t levels = ceil(log(max + 1) / log(K));//range [0 .. max] len=max+1
    const uint8_t height = levels - 1;//
    matrix_size = pow(K, levels);
    nodes = max + 1;
    edges = X.size();
    K = 2;
    storage.setMeta("K2tree_K", to_string(this->K));
    storage.setMeta("K2tree_NODES", to_string(this->nodes));
    storage.setMeta("K2tree_EDGES", to_string(this->edges));
    storage.setMeta("K2Tree_MATRIX_SIZE", to_string(this->matrix_size));

    //2°) ordena los puntos en el vector
    sort(zPoint.begin(), zPoint.end());
    //3°) procesda la raiz. Aqugí se agregan los chunk en uno solo para todos los puntos. Corta si llega a 1111
    Page<uint_t> buffer(storage.getD());
    buffer.createBlock(true);
    const uint_t chunkPerBuffer = storage.bitsPerPage() / 4;
    const uint_t bytesPorBuffer = (storage.getD() * storage.bitsPerWord()) / 8;
    uint8_t tempChunk = 0b00000000;
    uint8_t chunk[4] = {8, 4, 2, 1};
    uint8_t child;

    cout << "Procesando la raiz...(altura:" << (uint64_t) height << ")" << endl;

    for (const auto &point: zPoint) {
        child = getChild(point, height);
        cout << "point: " << std::bitset<16>(point) << " child: " << std::bitset<2>(getChild(point, height)) << endl;
        tempChunk |= chunk[child];
        if (tempChunk == 0b00001111) break;
    }
    //save root
    cout << "buffer.data()" << bitset<64>(buffer[0]) << endl;
    size_t chunkCount = 0;
    buffer.setChunk(chunkCount++, tempChunk);
    cout << chunkCount << "°chunk: " << std::bitset<4>(tempChunk) << endl;
    cout << "buffer.data()" << bitset<64>(buffer[0]) << endl;
    //4°) continua con los nodos internos para llenar T.
    uint64_t group;
    uint64_t groupBy;

    for (int i = height - 1; i >= 0; i--) {
        cout << "procesando sub-árboles de altura: " << i << endl;

        groupBy = getGroup(zPoint[0], i);
        tempChunk = 0;
        for (const auto &point: zPoint) {
            child = getChild(point, i);
            group = getGroup(point, i);
            if (group == groupBy) {
                tempChunk |= chunk[child];
            } else {
                //save group
                buffer.setChunk((chunkCount++) % chunkPerBuffer, tempChunk);
                cout << chunkCount << "°chunk: " << std::bitset<4>(tempChunk) << "(en buffer: "
                     << ((chunkCount) % chunkPerBuffer) << ")" << endl;
                cout << "buffer[" << ((chunkCount) % chunkPerBuffer) / (sizeof(uint_t) * 2) << "]="
                     << bitset<64>(buffer[((chunkCount) % chunkPerBuffer) / (sizeof(uint_t) * 2)]) << endl;
                cout << endl;
                if (chunkCount % chunkPerBuffer == 0) {
                    //save buffer
                    T_L.appendPage(buffer, storage.bitsPerPage());
                }
                //new group
                groupBy = group;
                tempChunk = chunk[child];
            }

            cout << "point: " << std::bitset<16>(point) << " group: " << group << " child: " << std::bitset<2>(child)
                 << endl;
        }
        //save last group of level i
        buffer.setChunk((chunkCount++) % chunkPerBuffer, tempChunk);
        cout << chunkCount << "°chunk: " << std::bitset<4>(tempChunk) << endl;
        cout << "buffer[" << ((chunkCount) % chunkPerBuffer) / (sizeof(uint_t) * 2) << "]="
             << bitset<64>(buffer[((chunkCount) % chunkPerBuffer) / (sizeof(uint_t) * 2)]) << endl;
        if (chunkCount % chunkPerBuffer == 0) {
            //save buffer
            T_L.appendPage(buffer, storage.bitsPerPage());
        }
    }
    //save last buffer (can be incompleted).
    cout << endl;

    T_L.appendPage(buffer, 4 * (chunkCount % chunkPerBuffer));
    cout << "guardando el último buffer (que está incompleto)" << endl;
    cout << "Imprimiendo el bitmap T:L" << endl;
    T_L.print();
    close();
}

template<typename uint_t>
ExternalK2Tree<uint_t>::~ExternalK2Tree() {
    storage.setMeta("K2tree_K", to_string(this->K));
    storage.setMeta("K2tree_NODES", to_string(nodes));
    storage.setMeta("K2tree_EDGES", to_string(edges));
    storage.setMeta("K2Tree_MATRIX_SIZE", to_string(matrix_size));
}

template<typename uint_t>
void ExternalK2Tree<uint_t>::close() {
    storage.close();
}

#endif //PRUEBASC___EXTERNALK2TREE_H
