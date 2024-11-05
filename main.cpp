// main.cpp
#include "b_tree.h"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

/**/

int main() {
    BTreeNode* root = nullptr;
    int max_keys;
    cout << "Ingrese el número máximo de claves por nodo: ";
    cin >> max_keys;

    string filename = "data.csv";

    // Cargar el archivo y registrar tiempo de carga
    readCSVAndInsert(filename, &root, max_keys);

    int opcion;
    do {
        cout << "\nMenú de opciones:\n";
        cout << "1. Cambiar número de claves máximas y recargar archivo\n";
        cout << "2. Imprimir árbol B\n";
        cout << "3. Buscar clave\n";
        cout << "4. Salir\n";
        cout << "Ingrese su opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Ingrese el número máximo de claves por nodo: ";
            cin >> max_keys;
            cout << "Recargando el archivo: " << filename << " con max_keys = " << max_keys << endl;

            delete root;  // Limpiar el árbol actual
            root = nullptr;  // Reinicializar el puntero raíz
            readCSVAndInsert(filename, &root, max_keys);  // Recargar con el nuevo max_keys
            break;

        case 2:
            if (root != nullptr) {
                cout << "Contenido del árbol B en orden:" << endl;
                printBTreeWithTime(root, max_keys);  // Llamada correcta con max_keys
                cout << endl;
            }
            else {
                cout << "El árbol está vacío." << endl;
            }
            break;

        case 3: {
            if (root != nullptr) {
                double keyToSearch;
                cout << "Ingrese el valor a buscar: ";
                cin >> keyToSearch;

                if (searchKeyWithTime(root, keyToSearch, max_keys)) {  // Llamada correcta con max_keys
                    cout << "Clave " << keyToSearch << "no encontrada." << endl;
                }
                else {
                    cout << "Clave " << keyToSearch << " encontrada." << endl;
                }
            }
            else {
                cout << "El árbol está vacío." << endl;
            }
            break;
        }
        case 4:
            cout << "Saliendo." << endl;
            break;

        default:
            cout << "Opción no válida." << endl;
            break;
        }
    } while (opcion != 4);
    delete root;
    return 0;
}
