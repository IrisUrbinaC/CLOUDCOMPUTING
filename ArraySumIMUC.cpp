// IRIS MONSERRAT URBINA CASAS
//A01795999
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <omp.h>

int main() {
    const int size = 1000;
    std::vector<int> arregloA(size);
    std::vector<int> arregloB(size);
    std::vector<int> arregloR(size);

    // Generar números aleatorios (como antes)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);

    for (int i = 0; i < size; ++i) {
        arregloA[i] = distrib(gen);
        arregloB[i] = distrib(gen);
    }

    // Imprimir los primeros 20 elementos 
    std::cout << "Primeros 20 elementos de Arreglo A: ";
    for (int i = 0; i < 20; ++i) std::cout << arregloA[i] << " ";
    std::cout << std::endl;
    std::cout << "Primeros 20 elementos de Arreglo B: ";
    for (int i = 0; i < 20; ++i) std::cout << arregloB[i] << " ";
    std::cout << std::endl;

     // Imprimir los primeros 20 elementos 
    std::cout << "Primeros 20 elementos de Arreglo A de la segunda mitad: ";
    for (int i = 500; i < 520; ++i) std::cout << arregloA[i] << " ";
    std::cout << std::endl;
    std::cout << "Primeros 20 elementos de Arreglo B de la segunda mitad: ";
    for (int i = 500; i < 520; ++i) std::cout << arregloB[i] << " ";
    std::cout << std::endl;


    int error_flag = 0;

    #pragma omp parallel num_threads(2) shared(arregloA, arregloB, arregloR, size, error_flag)
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int mitad = size / 2;
        int inicio, fin;
        std::vector<int> resultado_parcial; // Arreglo para resultados parciales

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
                resultado_parcial.resize(mitad); // Redimensionar el arreglo parcial
                std::cout << "\n Suma realizada por el hilo 0" << std::endl;
            } else if (thread_id == 1) {
                inicio = mitad;
                fin = size;
                resultado_parcial.resize(size - mitad); // Redimensionar el arreglo parcial
                std::cout << "\n Suma realizada por el hilo 1" << std::endl;
            } else {
                #pragma omp critical
                {
                    std::cerr << "Error: ID de hilo inesperado: " << thread_id << std::endl;
                    error_flag = 1;
                }
            }

            if (!error_flag) {
                for (int i = inicio; i < fin; ++i) {
                    resultado_parcial[i - inicio] = arregloA[i] + arregloB[i]; // Guardar en el arreglo parcial
                }

                // Imprimir resultados parciales ANTES de la barrera
                #pragma omp critical
                {
                  std::cout << "\nResultados parciales del hilo " << thread_id << ": ";
                  for (int val : resultado_parcial) std::cout << val << " ";
                  std::cout << std::endl;
                }

                #pragma omp barrier // Barrera para sincronizar antes de copiar al arreglo final
                if (thread_id == 0){
                   for (int i = 0; i < mitad; i++){
                    arregloR[i] = resultado_parcial[i];
                   }
                } else if (thread_id == 1){
                     for (int i = 0; i < (size-mitad); i++){
                        arregloR[mitad+i] = resultado_parcial[i];
                   }
                }
                #pragma omp barrier // Barrera para sincronizar antes de la impresion final

            }
        }
    }

    if (error_flag) {
        std::cerr << "Error detectado en la región paralela. Terminando el programa." << std::endl;
        return 1;
    }

    // Imprimir el arreglo resultado FINAL
    std::cout << "\nArreglo R (Resultado Final): ";
    for (int i = 0; i < 20; ++i) std::cout << arregloR[i] << " "; // Imprimir solo los primeros 100
    std::cout << "... (resto del arreglo)" << std::endl; // Indicar que hay más elementos
    std::cout << "\nsegunda mitad del arreglo" << std::endl; // Indicar que hay más elementos
    for (int i = 500; i < 520; ++i) std::cout << arregloR[i] << " "; // Imprimir solo los primeros 100 a partir de 500
    std::cout << "... (resto del arreglo)" << std::endl; // Indicar que hay más elementos

    return 0;
}
