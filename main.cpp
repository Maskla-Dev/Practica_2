/*
 * Practica 2
 * Seccion 1.1 Devolver el cambio
 * Luis Antonio Montoya Morales
 * */

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>

using uint = unsigned int;
using Cambio = std::vector<uint>;
using DineroCirculante = std::vector<uint>;
using ListaPagado = std::vector<uint>;
using TablaCambio = std::vector<Cambio>;
using micro_segundos = std::chrono::duration<double,std::micro>;
using Reloj = std::chrono::system_clock;

Cambio& devolverCambioVoraz(DineroCirculante&, uint);
Cambio& devolverCambioDinamico(DineroCirculante&, TablaCambio&, uint);
TablaCambio& hacerTabla(DineroCirculante&, uint);
uint encontrarValorMayor(ListaPagado&);
int mayorDenominacion(DineroCirculante&, uint, uint);
void imprimirCambioDevuelto(Cambio&);

int main() {
    std::chrono::system_clock::time_point inicio, final;
    micro_segundos duracion;
    DineroCirculante denominaciones = {500, 200, 100, 25, 10, 5, 1};
    ListaPagado cambio_a_devolver = {8, 25, 47, 356, 1025, 642};
    Cambio cambio_devuelto;
    TablaCambio tabla_cambio;
    //Ordena las denominaciones
    std::sort(denominaciones.begin(), denominaciones.end());
    std::reverse(denominaciones.begin(), denominaciones.end());
    std::cout << "********Inicia pruebas algoritmo voraz********" << std::endl;
    //Proceso de automatizacion de pruebas
    for(int i = 0; i < cambio_a_devolver.size(); ++i){
        inicio = Reloj::now();
        cambio_devuelto = devolverCambioVoraz(denominaciones, cambio_a_devolver[i]);
        final = Reloj::now();
        std::cout << "Para devolver " << cambio_a_devolver[i] << " -------------|" << std::endl;
        if(!cambio_a_devolver.empty()){
            std::sort(cambio_devuelto.begin(), cambio_devuelto.end());
            std::reverse(cambio_devuelto.begin(), cambio_devuelto.end());
            imprimirCambioDevuelto(cambio_devuelto);
        }
        else{
            std::cout << "Sin solucion" << std::endl;
        }
        duracion = final - inicio;
        std::cout << "Tiempo Ejecucion (micro segundos): " << duracion.count() << std::endl << std::endl;
        cambio_devuelto.clear();
    }
    std::reverse(denominaciones.begin(), denominaciones.end());
    std::cout << "********Inicia pruebas algoritmo dinamico********" << std::endl;
    inicio = Reloj::now();
    tabla_cambio = hacerTabla(denominaciones, encontrarValorMayor(cambio_a_devolver));
    final = Reloj::now();
    duracion = final - inicio;
    std::cout << "Tiempo Ejecucion creacion de tabla (micro segundos): " << duracion.count() << std::endl << std::endl;
    for(auto& i : cambio_a_devolver){
        std::cout << "Para devolver " << i << " -------------|" << std::endl;
        inicio = Reloj::now();
        cambio_devuelto = devolverCambioDinamico(denominaciones, tabla_cambio, i);
        final = Reloj::now();
        if(!cambio_a_devolver.empty()){
            std::sort(cambio_devuelto.begin(), cambio_devuelto.end());
            std::reverse(cambio_devuelto.begin(), cambio_devuelto.end());
            imprimirCambioDevuelto(cambio_devuelto);
        }
        else{
            std::cout << "Sin solucion" << std::endl;
        }
        duracion = final - inicio;
        std::cout << "Tiempo Ejecucion creacion de tabla (micro segundos): " << duracion.count() << std::endl << std::endl;
        cambio_devuelto.clear();
    }
    return 0;
}

TablaCambio& hacerTabla(DineroCirculante& denominaciones, uint valor_maximo){
    auto *tabla = new TablaCambio(denominaciones.size(), std::vector<uint>(valor_maximo + 1));
    int opcion1 = 0, opcion2 = 0, a;
    for(int i = 0; i < denominaciones.size(); ++i)
        (*tabla)[i][0] = 0;
    for(int i = 0; i < denominaciones.size(); ++i){
        for(int j = 1; j < valor_maximo + 1; ++j){
            if(i - 1 < 0)
                opcion1 = std::numeric_limits<int>::max();
            else
                opcion1 = (*tabla)[i - 1][j];
            if(j - static_cast<int>(denominaciones[i]) >= 0)
                opcion2 = 1 + (*tabla)[i][j - denominaciones[i]];
            else
                opcion2 = 0;
            (*tabla)[i][j] = (opcion1 < opcion2) ? opcion1 : opcion2;
        }
    }
    return *tabla;
}

uint encontrarValorMayor(ListaPagado& lista){
    uint mayor = lista.front();
    for(int i = 1; i < lista.size(); ++i)
        mayor = (mayor < lista[i]) ? lista[i] : mayor;
    return mayor;
}

Cambio& devolverCambioDinamico(DineroCirculante& denominaciones, TablaCambio& tabla, uint total_a_devolver){
    auto *cambio_devuelto = new Cambio;
    for(int i = denominaciones.size() - 1, j = total_a_devolver; i > 0 && j >= 0; --i){
        if(tabla[i][j] != tabla[i - 1][j]){
            if(!(j - static_cast<int>(denominaciones[i]) < 0)){
                if(tabla[i][j] == 1 + tabla[i][j - static_cast<int>(denominaciones[i])]){
                    if(tabla[i][j] == std::numeric_limits<int>::max()){
                        cambio_devuelto->clear();
                        return *cambio_devuelto;
                    }
                    else{
                        while(j - static_cast<int>(denominaciones[i]) >= 0){
                            cambio_devuelto->push_back(denominaciones[i]);
                            j -= denominaciones[i];
                        }
                    }
                }
            }
        }
    }
    return *cambio_devuelto;
}

void imprimirCambioDevuelto(Cambio& cambio){
    int contador = 0, denominacion = cambio[0];
    for(unsigned int i : cambio){
        if(denominacion == i)
            ++contador;
        else{
            std::cout << contador << " Monedas/Billetes de a " << denominacion << std::endl;
            contador = 1;
            denominacion = i;
        }
    }
    std::cout << contador << " Monedas/Billetes de a " << denominacion << std::endl;
}

Cambio& devolverCambioVoraz(DineroCirculante& denomincaciones, uint total_a_devolver){
    auto *cambio_devuelto = new Cambio;
    unsigned int por_pagar = 0;
    int x = 0;
    while(por_pagar != total_a_devolver){
        x = mayorDenominacion(denomincaciones, total_a_devolver, por_pagar);
        if(!x)
            return *cambio_devuelto;
        else{
            cambio_devuelto->push_back(x);
            por_pagar += x;
        }
    }
    return *cambio_devuelto;
}

int mayorDenominacion(DineroCirculante& denominaciones, uint total_a_devolver, uint por_pagar){
    for(auto& i : denominaciones)
        if(i + por_pagar <= total_a_devolver)
            return static_cast<int>(i);
    return 0;
}