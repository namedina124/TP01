#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// =====================
// ESTRUCTURAS
// =====================

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

// =====================
// CONSTANTES
// =====================

const int MAX_PROD = 100;
const int MAX_REP = 1000;

// =====================
// FUNCIONES
// =====================

int buscarProducto(const Producto productos[], int cantProd, const char sku[]) {
    for (int i = 0; i < cantProd; i++)
        if (strcmp(productos[i].sku, sku) == 0)
            return i;
    return -1;
}

const char* descripcionTipo(int tipo) {
    switch (tipo) {
        case 0: return "Electrónico";
        case 1: return "Mecánico";
        case 2: return "Mecatrónico";
        default: return "Desconocido";
    }
}

int criterioReparacion(const Reparacion& a, const Reparacion& b) {
    int c = strcmp(a.cliente, b.cliente);
    if (c != 0) return c;
    if (a.tipoProducto != b.tipoProducto)
        return a.tipoProducto - b.tipoProducto;
    return strcmp(a.sku, b.sku);
}

void ordenarReparaciones(Reparacion v[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (criterioReparacion(v[j], v[i]) < 0)
                swap(v[i], v[j]);
}

// =====================
// MAIN
// =====================

int main() {
    Producto productos[MAX_PROD];
    Reparacion reparaciones[MAX_REP];
    int cantProd = 0, cantRep = 0;

    ifstream archProd("productos.bin", ios::binary);
    if (!archProd) {
        cout << "Error al abrir productos.bin" << endl;
        return 1;
    }
    while (archProd.read((char*)&productos[cantProd], sizeof(Producto)))
        cantProd++;
    archProd.close();

    ifstream archRep("reparaciones.bin", ios::binary);
    if (!archRep) {
        cout << "Error al abrir reparaciones.bin" << endl;
        return 1;
    }
    while (archRep.read((char*)&reparaciones[cantRep], sizeof(Reparacion)))
        cantRep++;
    archRep.close();

    ordenarReparaciones(reparaciones, cantRep);

    char cliente[16];
    cout << "Ingrese cliente (EOF para finalizar): ";

    while (cin >> cliente) {
        bool encontrado = false;
        float gananciaTotal = 0.0f;

        for (int i = 0; i < cantRep; i++) {
            if (strcmp(reparaciones[i].cliente, cliente) == 0) {
                int posProd = buscarProducto(productos, cantProd, reparaciones[i].sku);
                if (posProd == -1) continue;

                encontrado = true;

                cout << "Cliente: " << reparaciones[i].cliente << endl;
                cout << "Tipo producto: " << descripcionTipo(reparaciones[i].tipoProducto) << endl;
                cout << "SKU: " << reparaciones[i].sku << endl;
                cout << "Producto: " << productos[posProd].descripcion << endl;
                cout << "Costo fijo: " << productos[posProd].costoFijo << endl;
                cout << "Costo directo: " << reparaciones[i].costoDirecto << endl;
                cout << "Presupuestado: " << reparaciones[i].presupuestado << endl;
                cout << "----------------------------------" << endl;

                gananciaTotal += reparaciones[i].presupuestado -
                                (productos[posProd].costoFijo + reparaciones[i].costoDirecto);
            }
        }

        if (!encontrado) {
            cout << "Cliente no encontrado." << endl;
        } else {
            cout << "Ganancia total con el cliente: " << gananciaTotal << endl;
        }

        cout << "\nIngrese cliente : ";
    }

    return 0;
}
