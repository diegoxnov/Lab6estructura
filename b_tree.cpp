// b_tree.cpp
#include "b_tree.h"
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;

BTreeNode* createNode(int max_keys) {
    return new BTreeNode(max_keys);
}

void insert(BTreeNode** root, double key, int max_keys) {
    if (*root == nullptr) {
        *root = createNode(max_keys);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    }
    else {
        if ((*root)->num_keys == max_keys) {
            BTreeNode* newRoot = createNode(max_keys);
            newRoot->children[0] = *root;
            splitChild(newRoot, 0, max_keys);
            *root = newRoot;
        }
        insertNonFull(*root, key, max_keys);
    }
}

void splitChild(BTreeNode* parent, int index, int max_keys) {
    BTreeNode* child = parent->children[index];
    BTreeNode* newChild = createNode(max_keys);
    int midIndex = (max_keys - 1) / 2;

    // Mover la mitad de las claves a newChild
    newChild->num_keys = max_keys - midIndex - 1;
    for (int i = 0; i < newChild->num_keys; i++) {
        newChild->keys[i] = child->keys[i + midIndex + 1];
    }

    // Mover los hijos a newChild si no es un nodo hoja
    if (child->children[0] != nullptr) {
        for (int i = 0; i <= newChild->num_keys; i++) {
            newChild->children[i] = child->children[i + midIndex + 1];
        }
    }

    // Ajustar el número de claves en el nodo hijo original
    child->num_keys = midIndex;

    // Insertar el nuevo hijo en el nodo padre
    for (int i = parent->num_keys; i >= index + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;

    // Subir la clave central al padre
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[midIndex];
    parent->num_keys++;
}

void insertNonFull(BTreeNode* node, double key, int max_keys) {
    int i = node->num_keys - 1;
    if (node->children[0] == nullptr) {
        insertKey(node, key);
        return;
    }
    while (i >= 0 && node->keys[i] > key) {
        i--;
    }
    i++;
    if (node->children[i]->num_keys == max_keys) {
        splitChild(node, i, max_keys);
        if (node->keys[i] < key) {
            i++;
        }
    }
    insertNonFull(node->children[i], key, max_keys);
}

void insertKey(BTreeNode* node, double key) {
    int i = node->num_keys - 1;
    while (i >= 0 && node->keys[i] > key) {
        node->keys[i + 1] = node->keys[i];
        i--;
    }
    node->keys[i + 1] = key;
    node->num_keys++; // Aumenta el número de claves en el nodo
}
// Función de impresión con temporizador
void printBTreeWithTime(BTreeNode* root, int max_keys) {
    auto start = chrono::high_resolution_clock::now();
    printBTree(root);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    ofstream outfile("execution_times.csv", ios::app);
    if (outfile.is_open()) {
        outfile << max_keys << "," << duration.count() << "," << "Imprimir" << endl;
        outfile.close();
    }
}

void printBTree(BTreeNode* root) {
    if (root != nullptr) {
        for (int i = 0; i < root->num_keys; i++) {
            printBTree(root->children[i]); // Imprimir el subárbol izquierdo
            cout << fixed << setprecision(5) << root->keys[i] << " ";
        }
        printBTree(root->children[root->num_keys]); // Imprimir el subárbol derecho
    }
}

// Función para cargar archivo y registrar tiempo de carga
void readCSVAndInsert(const string& filename, BTreeNode** root, int max_keys) {
    auto start = chrono::high_resolution_clock::now();

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Leer encabezado si existe
    while (getline(file, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            double value = stod(line.substr(pos + 1));
            insert(root, value, max_keys);
        }
    }
    file.close();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    ofstream outfile("execution_times.csv", ios::app);
    if (outfile.is_open()) {
        outfile << max_keys << ";" << duration.count() << "," << "Cargar" << endl;
        outfile.close();
    }
}
// Función de búsqueda con temporizador
bool searchKeyWithTime(BTreeNode* root, double key, int max_keys) {
    auto start = chrono::high_resolution_clock::now();
    bool found = searchKey(root, key);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    ofstream outfile("execution_times.csv", ios::app);
    if (outfile.is_open()) {
        outfile << max_keys << "," << duration.count() << "Buscar" << endl;
        outfile.close();
    }

    return found;
}

bool searchKey(BTreeNode* root, double key) {
    if (root == nullptr) return false; // Asegurarse de que root no es nullptr

    int i = 0;
    while (i < root->num_keys && key > root->keys[i]) {
        i++;
    }
    if (i < root->num_keys && key == root->keys[i]) {
        return true;
    }
    if (root->children[0] == nullptr) {
        return false;
    }
    return searchKey(root->children[i], key);
}
