#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define MAX 10 // Tamaño máximo de la matriz

// Prototipos de funciones relacionadas con funciones matemáticas
long double f1(long double x);
long double f2(long double x);
long double f3(long double x);
long double f4(long double x);
long double secante(long double p0, long double p1, long double (*f)(long double), int kmax, long double eps);
long double biseccion(long double a, long double b, long double (*f)(long double), int kmax, long double eps);

// Prototipos de funciones relacionadas con matrices
void leerMatriz(float matriz[MAX][MAX], float vector[MAX], int n);
void mostrarMatriz(float matriz[MAX][MAX], float vector[MAX], int n);
int esDominanteDiagonal(float matriz[MAX][MAX], int n);
float calcularDeterminante(float matriz[MAX][MAX], int n);
void corregirElemento(float matriz[MAX][MAX], int n);

// Prototipo del método Jacobi
void metodoJacobi(float A[MAX][MAX], float b[MAX], float x[MAX], int n, int kmax, float eps);

int main() {
    bool continuar = true;
    while (continuar) {
        int opcion;
        cout << "Programa hecho por Garcia Reyes Arely Lizeth, Hernandez Barraza Alejandro y Lozano Perez Johan Andres\n";
        cout << "Seleccione una opcion:\n";
        cout << "1. Operaciones con funciones matematicas\n";
        cout << "2. Operaciones con matrices\n";
        cout << "3. Salir\n";
        cin >> opcion;

        if (opcion == 1) {
            // Operaciones con funciones matemáticas
            int metodo, kmax, funcion;
            long double p0, p1, a, b, eps, raiz;

            cout << "Seleccione el metodo para encontrar la raiz:\n";
            cout << "1. Metodo de la secante\n";
            cout << "2. Metodo de biseccion\n";
            cin >> metodo;

            cout << "Seleccione la funcion:\n";
            cout << "1. f(x) = x^2 * cos(x) - 2x\n";
            cout << "2. f(x) = (6 - 2/x^2) * (e^(2+x)/4) + 1\n";
            cout << "3. f(x) = x^3 - 3*sin(x^2) + 1\n";
            cout << "4. f(x) = x^3 + 6x^2 + 9.4x + 2.5\n";
            cin >> funcion;

            long double (*f)(long double);
            switch (funcion) {
                case 1: f = f1; break;
                case 2: f = f2; break;
                case 3: f = f3; break;
                case 4: f = f4; break;
                default:
                    cout << "Funcion no valida.\n";
                    continue;
            }

            if (metodo == 1) { // Secante
                cout << "Ingrese el intervalo inicial (p0 y p1):\n";
                cin >> p0 >> p1;
                cout << "Ingrese la tolerancia de error (eps):\n";
                cin >> eps;
                cout << "Ingrese el maximo numero de iteraciones (kmax):\n";
                cin >> kmax;
                raiz = secante(p0, p1, f, kmax, eps);
                cout << "La raiz aproximada encontrada es: " << raiz << endl;
            } else if (metodo == 2) { // Biseccion
                cout << "Ingrese el intervalo (a y b):\n";
                cin >> a >> b;
                cout << "Ingrese la tolerancia de error (eps):\n";
                cin >> eps;
                cout << "Ingrese el maximo numero de iteraciones (kmax):\n";
                cin >> kmax;
                raiz = biseccion(a, b, f, kmax, eps);
                cout << "La raiz aproximada encontrada es: " << raiz << endl;
            } else {
                cout << "Metodo no valido.\n";
            }
        } else if (opcion == 2) {
            // Operaciones con matrices
            float matriz[MAX][MAX], vector[MAX];
            int n;

            cout << "Introduce la dimension de la matriz (n x n): ";
            cin >> n;
            leerMatriz(matriz, vector, n);

            char subopcion;
            do {
                cout << "Seleccione una operacion:\n";
                cout << "a. Mostrar la matriz y el vector\n";
                cout << "b. Verificar si la matriz es dominante diagonal\n";
                cout << "c. Calcular el determinante\n";
                cout << "d. Corregir un elemento de la matriz\n";
                cout << "e. Resolver el sistema de ecuaciones con el metodo de Jacobi\n";
                cout << "f. Regresar al menu principal\n";
                cin >> subopcion;

                switch (subopcion) {
                    case 'a':
                        mostrarMatriz(matriz, vector, n);
                        break;
                    case 'b':
                        if (esDominanteDiagonal(matriz, n)) {
                            cout << "La matriz es dominante diagonal.\n";
                        } else {
                            cout << "La matriz no es dominante diagonal.\n";
                        }
                        break;
                    case 'c': {
                        float determinante = calcularDeterminante(matriz, n);
                        cout << "Determinante: " << determinante << endl;
                        break;
                    }
                    case 'd':
                        corregirElemento(matriz, n);
                        break;
                    case 'e': {
                        float x[MAX] = {0}; // Vector de incógnitas inicial
                        int kmax;
                        float eps;
                        cout << "Ingrese el numero maximo de iteraciones (kmax): ";
                        cin >> kmax;
                        cout << "Ingrese la tolerancia de error (eps): ";
                        cin >> eps;
                        metodoJacobi(matriz, vector, x, n, kmax, eps);
                        break;
                    }
                    case 'f':
                        break;
                    default:
                        cout << "Opcion no valida.\n";
                }
            } while (subopcion != 'f');
        } else if (opcion == 3) {
            continuar = false;
        } else {
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }

    return 0;
}

// Funciones de funciones matemáticas
long double f1(long double x) { return pow(x, 2) * cos(x) - 2 * x; }
long double f2(long double x) { return (6 - 2 / pow(x, 2)) * (exp(2 + x) / 4) + 1; }
long double f3(long double x) { return pow(x, 3) - 3 * sin(pow(x, 2)) + 1; }
long double f4(long double x) { return pow(x, 3) + 6 * pow(x, 2) + 9.4 * x + 2.5; }

long double secante(long double p0, long double p1, long double (*f)(long double), int kmax, long double eps) {
    long double q0 = f(p0), q1 = f(p1);
    for (int k = 0; k < kmax; ++k) {
        long double p = p1 - q1 * (p1 - p0) / (q1 - q0);
        if (fabs(p - p1) < eps) return p;
        p0 = p1;
        q0 = q1;
        p1 = p;
        q1 = f(p);
    }
    return p1;
}

long double biseccion(long double a, long double b, long double (*f)(long double), int kmax, long double eps) {
    long double fa = f(a), fb = f(b);
    if (fa * fb > 0) {
        cout << "La funcion no cambia de signo en el intervalo proporcionado.\n";
        return a;
    }
    for (int k = 0; k < kmax; ++k) {
        long double p = (a + b) / 2, fp = f(p);
        if (fabs(fp) < eps || fabs(b - a) < eps) return p;
        if (fa * fp < 0) {
            b = p;
            fb = fp;
        } else {
            a = p;
            fa = fp;
        }
    }
    return (a + b) / 2;
}

// Funciones relacionadas con matrices
void leerMatriz(float matriz[MAX][MAX], float vector[MAX], int n) {
    cout << "Introduce los coeficientes de la matriz (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << "Elemento [" << i + 1 << "][" << j + 1 << "]: ";
            cin >> matriz[i][j];
        }
    }
    cout << "Introduce los valores del vector independiente:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Elemento [" << i + 1 << "]: ";
        cin >> vector[i];
    }
}

void mostrarMatriz(float matriz[MAX][MAX], float vector[MAX], int n) {
    cout << "Matriz:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(8) << matriz[i][j] << " ";
        }
        cout << "| " << vector[i] << endl;
    }
}

int esDominanteDiagonal(float matriz[MAX][MAX], int n) {
    for (int i = 0; i < n; ++i) {
        float suma = 0;
        for (int j = 0; j < n; ++j) {
            if (i != j) suma += fabs(matriz[i][j]);
        }
        if (fabs(matriz[i][i]) <= suma) return 0;
    }
    return 1;
}

float calcularDeterminante(float matriz[MAX][MAX], int n) {
    // Se utiliza eliminación gaussiana para calcular el determinante
    float det = 1;
    for (int i = 0; i < n; ++i) {
        if (matriz[i][i] == 0) {
            for (int k = i + 1; k < n; ++k) {
                if (matriz[k][i] != 0) {
                    for (int j = 0; j < n; ++j) swap(matriz[i][j], matriz[k][j]);
                    det = -det;
                    break;
                }
            }
        }
        if (matriz[i][i] == 0) return 0;
        det *= matriz[i][i];
        for (int k = i + 1; k < n; ++k) {
            float factor = matriz[k][i] / matriz[i][i];
            for (int j = i; j < n; ++j) matriz[k][j] -= factor * matriz[i][j];
        }
    }
    return det;
}

void corregirElemento(float matriz[MAX][MAX], int n) {
    int fila, columna;
    float nuevoValor;
    cout << "Introduce la fila (1 a " << n << "): ";
    cin >> fila;
    cout << "Introduce la columna (1 a " << n << "): ";
    cin >> columna;
    cout << "Introduce el nuevo valor: ";
    cin >> nuevoValor;
    matriz[fila - 1][columna - 1] = nuevoValor;
    cout << "Elemento actualizado correctamente.\n";
}

// Implementación del método de Jacobi
void metodoJacobi(float A[MAX][MAX], float b[MAX], float x[MAX], int n, int kmax, float eps) {
    float xNuevo[MAX] = {0};
    for (int k = 0; k < kmax; ++k) {
        for (int i = 0; i < n; ++i) {
            float suma = 0;
            for (int j = 0; j < n; ++j) {
                if (i != j) suma += A[i][j] * x[j];
            }
            xNuevo[i] = (b[i] - suma) / A[i][i];
        }
        // Calcular el error
        float error = 0;
        for (int i = 0; i < n; ++i) {
            error += pow(xNuevo[i] - x[i], 2);
            x[i] = xNuevo[i];
        }
        error = sqrt(error);
        // Mostrar resultados intermedios
        cout << "Iteracion " << k + 1 << ": ";
        for (int i = 0; i < n; ++i) cout << x[i] << " ";
        cout << "Error: " << error << endl;
        if (error < eps) break;
    }
    cout << "Solucion aproximada:\n";
    for (int i = 0; i < n; ++i) cout << "x[" << i + 1 << "] = " << x[i] << endl;
}
