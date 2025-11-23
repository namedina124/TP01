#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

// ===========================
// ESTRUCTURAS DE DATOS
// ===========================

struct Producto {
    char SKU[11];         // 10 caracteres + '\0'
    char descripcion[21]; // 20 caracteres + '\0'
    float costoFijo;
};

struct Reparacion {
    char cliente[16];     // 15 caracteres + '\0'
    int tipoProducto;     // 0, 1 o 2
    char SKU[11];         // 10 caracteres + '\0'
    float costoDirecto;
    float presupuestado;
};

// ===========================
// FUNCIONES TEMPLATE
// ===========================

// Ordenamiento genérico (por selección)
template <typename T>
void ordenar(vector<T>& v, bool (*criterio)(const T&, const T&)) {
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = i + 1; j < v.size(); j++) {
            if (criterio(v[j], v[i])) {
                swap(v[i], v[j]);
            }
        }
    }
}

// Criterio de ordenación de Reparaciones
bool criterioReparacion(const Reparacion& a, const Reparacion& b) {
    int cmp = strcmp(a.cliente, b.cliente);
    if (cmp < 0) return true;
    if (cmp > 0) return false;
    if (a.tipoProducto < b.tipoProducto) return true;
    if (a.tipoProducto > b.tipoProducto) return false;
    return strcmp(a.SKU, b.SKU) < 0;
}

// Búsqueda lineal ordenada genérica
template <typename T>
int buscarLineal(const vector<T>& v, const char* clave, bool (*criterio)(const T&, const char*)) {
    for (size_t i = 0; i < v.size(); i++) {
        if (criterio(v[i], clave))
            return (int)i;
    }
    return -1;
}

// Criterio de comparación para buscar por cliente
bool criterioBusquedaCliente(const Reparacion& r, const char* nombre) {
    return strcmp(r.cliente, nombre) == 0;
}

// ===========================
// FUNCIONES AUXILIARES
// ===========================

string tipoProductoTexto(int tipo) {
    switch (tipo) {
        case 0: return "Electrónico";
        case 1: return "Mecánico";
        case 2: return "Mecatrónico";
        default: return "Desconocido";
    }
}

// ===========================
// PROGRAMA PRINCIPAL
// ===========================

int main() {
    vector<Producto> productos;
    vector<Reparacion> reparaciones;

    //Leer archivo de productos
    ifstream archProd("productos.bin", ios::binary);
    if (!archProd) {
        cout << "No se pudo abrir el archivo productos.bin" << endl;
        return 1;
    }

    Producto p;
    while (archProd.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        productos.push_back(p);
    }
    archProd.close();

    //Leer archivo de reparaciones
    ifstream archRep("reparaciones.bin", ios::binary);
    if (!archRep) {
        cout << "No se pudo abrir el archivo reparaciones.bin" << endl;
        return 1;
    }

    Reparacion r;
    while (archRep.read(reinterpret_cast<char*>(&r), sizeof(Reparacion))) {
        reparaciones.push_back(r);
    }
    archRep.close();

    cout << "Productos leídos: " << productos.size() << endl;
    cout << "Reparaciones leídas: " << reparaciones.size() << endl;

    // Ordenar las reparaciones
    ordenar(reparaciones, criterioReparacion);

    // Ciclo para pedir clientes 
    cout << endl << "=== Consulta de reparaciones por cliente ===" << endl;
    cout << "(Ctrl+Z y Enter para finalizar)" << endl;

    string nombreCliente;
    while (true) {
        cout << endl << "Ingrese nombre del cliente: ";
        if (!getline(cin, nombreCliente)) {
            cout << "Fin del programa." << endl;
            break; // EOF -> termina el programa
        }

        if (nombreCliente.empty()) continue;

        // Buscar si el cliente existe
        int pos = buscarLineal(reparaciones, nombreCliente.c_str(), criterioBusquedaCliente);

        if (pos == -1) {
            cout << "Cliente no encontrado." << endl;
            continue;
        }

        cout << endl << "Reparaciones del cliente: " << nombreCliente << endl;
        cout << "-------------------------------------------------------------" << endl;

        float gananciaTotal = 0.0f;

        // Mostrar todas las reparaciones del cliente
        for (const auto& r : reparaciones) {
            if (strcmp(r.cliente, nombreCliente.c_str()) == 0) {
                // Buscar el producto correspondiente al SKU
                Producto prod{};
                bool encontrado = false;
                for (const auto& p : productos) {
                    if (strcmp(p.SKU, r.SKU) == 0) {
                        prod = p;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado) continue;

                // Mostrar información
                cout << "Cliente: " << r.cliente << endl;
                cout << "Tipo producto: " << tipoProductoTexto(r.tipoProducto) << endl;
                cout << "SKU: " << r.SKU << endl;
                cout << "Producto: " << prod.descripcion << endl;
                cout << "Costo fijo: $" << prod.costoFijo << endl;
                cout << "Costo directo: $" << r.costoDirecto << endl;
                cout << "Presupuestado: $" << r.presupuestado << endl;
                cout << "-------------------------------------------------------------" << endl;

                // Calcular ganancia
                float ganancia = r.presupuestado - (prod.costoFijo + r.costoDirecto);
                gananciaTotal += ganancia;
            }
        }

        cout << "Ganancia total con " << nombreCliente << ": $" << gananciaTotal << endl;
    }

    return 0;
}
