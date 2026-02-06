#include <iostream>
#include <fstream>
#include <cstring>
#include "rwstring.hpp"

using namespace std;

// ==================================================
// ESTRUCTURAS
// ==================================================

struct Producto {
    char sku[11];
    char descripcion[21];
    float costoFijo;
};

struct Reparacion {
    char cliente[16];
    int tipoProducto;
    char sku[11];
    float costoDirecto;
    float presupuestado;
};

// ==================================================
// CONSTANTES
// ==================================================

const int MAX_PROD = 10;
const int MAX_REP  = 200;

// ==================================================
// FUNCIONES AUXILIARES
// ==================================================

const char* descTipo(int t) {
    switch (t) {
        case 0: return "Electrónico";
        case 1: return "Mecánico";
        case 2: return "Mecatrónico";
        default: return "Desconocido";
    }
}

int buscarProducto(Producto v[], int n, const char* sku) {
    for (int i = 0; i < n; i++)
        if (strcmp(v[i].sku, sku) == 0)
            return i;
    return -1;
}

// ==================================================
// MAIN
// ==================================================

int main() {

    Producto productos[MAX_PROD];
    Reparacion reparaciones[MAX_REP];
    int cantProd = 0, cantRep = 0;

    // ===============================
    // LECTURA PRODUCTOS
    // ===============================
    ifstream fprod("productos.bin", ios::binary);
    if (!fprod) {
        cout << "Error productos.bin\n";
        return 1;
    }

    while (cantProd < MAX_PROD && fprod) {
        string s;

        s = readstring(fprod, 10);
        if (!fprod) break;
        strcpy(productos[cantProd].sku, s.c_str());

        s = readstring(fprod, 20);
        strcpy(productos[cantProd].descripcion, s.c_str());

        fprod.read((char*)&productos[cantProd].costoFijo, sizeof(float));
        cantProd++;
    }
    fprod.close();

    // ===============================
    // LECTURA REPARACIONES
    // ===============================
    ifstream frep("reparaciones.bin", ios::binary);
    if (!frep) {
        cout << "Error reparaciones.bin\n";
        return 1;
    }

    while (cantRep < MAX_REP && frep) {
        string s;

        s = readstring(frep, 15);
        if (!frep) break;
        strcpy(reparaciones[cantRep].cliente, s.c_str());

        frep.read((char*)&reparaciones[cantRep].tipoProducto, sizeof(int));

        s = readstring(frep, 10);
        strcpy(reparaciones[cantRep].sku, s.c_str());

        frep.read((char*)&reparaciones[cantRep].costoDirecto, sizeof(float));
        frep.read((char*)&reparaciones[cantRep].presupuestado, sizeof(float));

        cantRep++;
    }
    frep.close();

    // ===============================
    // CONSULTA POR CLIENTE
    // ===============================
    char clienteBuscado[16];

    cout << "Ingrese el cliente a consultar: ";
    while (cin.getline(clienteBuscado, 16)) {

        bool encontrado = false;
        float gananciaTotal = 0;

        cout << "\nCliente         Tipo Prod.  SKU       Producto         "
             << "Costo Fijo Costo Direc. Presupuestado\n";

        for (int i = 0; i < cantRep; i++) {
            if (strcmp(reparaciones[i].cliente, clienteBuscado) == 0) {

                int pos = buscarProducto(productos, cantProd, reparaciones[i].sku);
                if (pos == -1) continue;

                encontrado = true;

                float ganancia =
                    reparaciones[i].presupuestado -
                    (productos[pos].costoFijo + reparaciones[i].costoDirecto);

                gananciaTotal += ganancia;

                cout << reparaciones[i].cliente << "  "
                     << descTipo(reparaciones[i].tipoProducto) << "  "
                     << reparaciones[i].sku << "  "
                     << productos[pos].descripcion << "  "
                     << productos[pos].costoFijo << "  "
                     << reparaciones[i].costoDirecto << "  "
                     << reparaciones[i].presupuestado << endl;
            }
        }

        if (!encontrado) {
            cout << "Cliente no encontrado.\n";
        } else {
            cout << "Ganancia total: " << gananciaTotal << endl;
        }

        cout << "\nIngrese el cliente a consultar: ";
    }

    return 0;
}
