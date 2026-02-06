# Trabajo Práctico Nº1 – Algoritmos y Estructuras de Datos

##  Descripción

Este trabajo práctico implementa un sistema de consulta para una empresa de reparaciones que opera sobre dos archivos binarios:

- `productos.bin`, que contiene información fija de los productos reparados.
- `reparaciones.bin`, que almacena las reparaciones realizadas a distintos clientes.

El programa:

- Lee ambos archivos binarios respetando el formato de serialización de strings de longitud fija.
- Almacena los datos en arreglos de estructuras.
- Solicita repetidamente el nombre de un cliente por teclado.
- Lista todas las reparaciones asociadas a ese cliente, mostrando:
  - Cliente  
  - Tipo de producto (descripción)  
  - SKU  
  - Nombre del producto  
  - Costo fijo  
  - Costo directo  
  - Monto presupuestado
- Calcula e informa la ganancia total obtenida con dicho cliente.
- Informa adecuadamente cuando el cliente no existe en los registros.

El programa finaliza cuando se ingresa la señal de fin de archivo (EOF).

##  Conceptos aplicados

- Uso de estructuras (`struct`).
- Manejo de arreglos estáticos en C/C++.
- Lectura de archivos binarios.
- Búsqueda lineal en arreglos.
- Conversión de códigos numéricos a descripciones textuales.
- Manejo de strings serializados de longitud fija mediante `rwstring.hpp`.
- Cálculo de resultados a partir de datos leídos de archivos.

# Integrante – Grupo 6
-Medina Nahuel
