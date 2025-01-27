// IRIS MONSERRAT URBINA CASAS
//A01795999
#include <iostream>
#include <vector>
#include <random> // Para generar números aleatorios
#include <numeric>
#include <omp.h>

int main() {
    const int size = 1000;
    std::vector<int> arregloA(size);
    std::vector<int> arregloB(size);
    std::vector<int> arregloR(size);

    // Generar números aleatorios
    std::random_device rd; // Obtener una semilla aleatoria del sistema
    std::mt19937 gen(rd()); // Generador de números pseudoaleatorios Mersenne Twister
    std::uniform_int_distribution<> distrib(1, 100); // Distribución uniforme entre 1 y 100 (inclusive)

    for (int i = 0; i < size; ++i) {
        arregloA[i] = distrib(gen);
        arregloB[i] = distrib(gen);
    }

    // Imprimir los primeros 10 elementos de los arreglos (opcional, para no saturar la consola)
    std::cout << "Primeros 100 de 1000 elementos aleatorios de Arreglo A: ";
    for (int i = 0; i < 100; ++i) std::cout << arregloA[i] << " ";
    std::cout << std::endl;
    std::cout << "Primeros 100 de 1000 elementos aleatorios de Arreglo B: ";
    for (int i = 0; i < 100; ++i) std::cout << arregloB[i] << " ";
    std::cout << std::endl;


    int error_flag = 0;

    #pragma omp parallel num_threads(2) shared(arregloA, arregloB, arregloR, size, error_flag)
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int mitad = size / 2;
        int inicio, fin;

        if (num_threads != 2) {
            #pragma omp critical
            {
                std::cerr << "Error: Se requieren exactamente 2 hilos. Hay " << num_threads << std::endl;
                error_flag = 1;
            }
        } else {
            if (thread_id == 0) {
                inicio = 0;
                fin = mitad;
                std::cout << "Suma realizada por el hilo 0" << std::endl;
            } else if (thread_id == 1) {
                inicio = mitad;
                fin = size;
                std::cout << "Suma realizada por el hilo 1" << std::endl;
            } else {
                #pragma omp critical
                {
                    std::cerr << "Error: ID de hilo inesperado: " << thread_id << std::endl;
                    error_flag = 1;
                }
            }

            if (!error_flag) {
                for (int i = inicio; i < fin; ++i) {
                    arregloR[i] = arregloA[i] + arregloB[i];
                }
            }
        }
        #pragma omp barrier
    }

        if (error_flag) {
        std::cerr << "Error detectado en la región paralela. Terminando el programa." << std::endl;
        return 1; // Salir con código de error
    }

    // Imprimir los primeros 10 elementos del arreglo resultado (opcional)
    std::cout << "\nPrimeros 100 elementos de 1000 del Arreglo R: ";
    for (int i = 0; i < 100; ++i) std::cout << arregloR[i] << " ";
    std::cout << std::endl;

    return 0;
}