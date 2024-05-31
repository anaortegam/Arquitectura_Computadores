#ifndef PROYECTO_ARQUITECTURA_PROGARGS_HPP
#define PROYECTO_ARQUITECTURA_PROGARGS_HPP
#include "../constantes.hpp"

struct VectorAceleracion {
    //La estructura VectorAceleracion contiene: aceleración por eje (x,y,z)
    double x;
    double y;
    double z;
};

struct Particula {
    //La estructura Particula contiene: posición por eje (px,py,pz); vector hv por eje (hvx, hvy, hvz); velocidad por eje (vx, vy, vz);
    //                                  densidad; vector de la aceleración; id; índice de su bloque por eje (i,j,z)
    double px, py, pz;
    double hvx, hvy, hvz;
    double vx, vy, vz;
    double dens;
    VectorAceleracion aceleracion_externa;
    int id;
    int i, j, z;
};

struct ConstantesGlobales {
    //La estructura ConstantesGlobales contiene: longitud de suavizado (suavidad); masa de una partícula
    double suavidad;
    double masa;
};

//Declaración de las funciones en progargs.cpp
void cargarParticula(std::vector<Particula>& particulas, std::ifstream& file, int np, int& contador);
bool cargarDatos(const std::string& filename, std::vector<Particula>& particulas, float& ppm, int& np);
std::vector<ConstantesGlobales> parametrosSimulacion(float ppm, double& masa, double& suavidad, std::vector<ConstantesGlobales>& constantes);
bool Numero(const std::string& str);
bool Digito(const std::string& str);
int sim_main(int argc, const std::vector<std::string>& argv);

#endif //PROYECTO_ARQUITECTURA_PROGARGS_HPP