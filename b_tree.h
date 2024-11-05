// b_tree.h
#ifndef B_TREE_H
#define B_TREE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

struct BTreeNode {
    int num_keys;
    int max_keys; // Número máximo de claves
    double* keys; // Puntero para un tamaño dinámico
    BTreeNode** children;

    BTreeNode(int max_keys) : num_keys(0), max_keys(max_keys) {
        keys = new double[max_keys];            // Crear array dinámico para claves
        children = new BTreeNode * [max_keys + 1]; // Crear array dinámico para hijos
        for (int i = 0; i < max_keys + 1; i++) {
            children[i] = nullptr;               // Inicializar hijos a nullptr
        }
    }

    ~BTreeNode() {
        delete[] keys;
        for (int i = 0; i < max_keys + 1; i++) {
            delete children[i]; // Liberar memoria de hijos
        }
        delete[] children;
    }
};

BTreeNode* createNode(int max_keys);
void insert(BTreeNode** root, double key, int max_keys);
void splitChild(BTreeNode* parent, int index, int max_keys);
void insertNonFull(BTreeNode* node, double key, int max_keys);
void insertKey(BTreeNode* node, double key);
void printBTree(BTreeNode* root);
void readCSVAndInsert(const string& filename, BTreeNode** root, int max_keys);
bool searchKey(BTreeNode* root, double key);
/*temporizadores*/
bool searchKeyWithTime(BTreeNode* root, double key, int max_keys);
void printBTreeWithTime(BTreeNode* root, int max_keys);


#endif
