/*
 * Practica 2
 * Seccion 1.1 Devolver el cambio
 * Luis Antonio Montoya Morales
 * */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>

using uint = unsigned int;
using Cambio = std::vector<uint>;
using DineroCirculante = std::vector<uint>;
using micro_segundos = std::chrono::duration<double,std::micro>;
using Tiempo_micras = std::vector<micro_segundos>;
using Reloj = std::chrono::system_clock;

Cambio* devolverCambio(DineroCirculante&, uint);
void obtenerDenomincaciones(DineroCirculante&, std::vector<std::string>&, uint);
int mayorDenominacion(DineroCirculante&, uint, uint);
void imprimirCambioDevuelto(Cambio&);

int main(int argc, char *argv[]) {
    std::chrono::system_clock::time_point inicio, final;
    DineroCirculante denominaciones;
    Cambio *cambio_a_devolver = nullptr;
    std::vector<std::string> raw_args;
    unsigned int por_pagar = 0;
    if(argc > 1){
        raw_args.assign(argv + 1, argv + argc);
        obtenerDenomincaciones(denominaciones, raw_args, std::stoi(raw_args[0]));
        //Ordena las denominaciones
        std::sort(denominaciones.begin(), denominaciones.end());
        std::reverse(denominaciones.begin(), denominaciones.end());
        //Proceso de automatizacion de pruebas
        for(int i = 0; i < std::stoi(raw_args[1]); ++i){
            por_pagar = std::stoi(raw_args[3 + std::stoi(raw_args[1]) + i]);
            inicio = Reloj::now();
            cambio_a_devolver = devolverCambio(denominaciones, por_pagar);
            final = Reloj::now();
            std::cout << "Para devolver " << por_pagar << " -------------|" << std::endl;
            if(!cambio_a_devolver->empty()){
                std::sort(cambio_a_devolver->begin(), cambio_a_devolver->end());
                std::reverse(cambio_a_devolver->begin(), cambio_a_devolver->end());
                imprimirCambioDevuelto(*cambio_a_devolver);
            }
            else{
                std::cout << "Sin solucion" << std::endl;
            }
            std::cout << "Tiempo Ejecucion (micro segundos): " << (final - inicio).count() << std::endl << std::endl;
            delete cambio_a_devolver;
            cambio_a_devolver = nullptr;
        }
    }
    else
        std::cout << "Sin argumentos por leer, saliendo del programa" << std::endl;
    return 0;
}

void imprimirCambioDevuelto(Cambio& cambio){
    int contador = 0, denominacion = cambio[0];
    for(int i = 0; i < cambio.size(); ++i){
        if(denominacion == cambio[i])
            ++contador;
        else{
            std::cout << contador << " Monedas/Billetes de a " << denominacion << std::endl;
            contador = 1;
            denominacion = cambio[i];
        }
    }
    std::cout << contador << " Monedas/Billetes de a " << denominacion << std::endl;
}

void obtenerDenomincaciones(DineroCirculante& denominaciones, std::vector<std::string>& raw_args, uint elementos){
    for(int i = 0; i <= elementos; ++i)
        denominaciones.push_back(std::stoi(raw_args[2 + i]));
}

Cambio* devolverCambio(DineroCirculante& denomincaciones, uint total_a_devolver){
    auto *cambio_devuelto = new Cambio;
    unsigned int por_pagar = 0;
    int x = 0;
    while(por_pagar != total_a_devolver){
        x = mayorDenominacion(denomincaciones, total_a_devolver, por_pagar);
        if(!x)
            return cambio_devuelto;
        else{
            cambio_devuelto->push_back(x);
            por_pagar += x;
        }
    }
    return cambio_devuelto;
}

int mayorDenominacion(DineroCirculante& denominaciones, uint total_a_devolver, uint por_pagar){
    for(auto& i : denominaciones)
        if(i + por_pagar <= total_a_devolver)
            return static_cast<int>(i);
    return 0;
}