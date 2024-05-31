
// En grid.hpp
#ifndef GRID_HPP
#define GRID_HPP
#include <vector>
#include "../constantes.hpp"
#include "progargs.hpp"

struct ConstantesMalla {
    //La estructura ConstantesMalla contiene: número de bloques por eje (nx,ny,nz); tamaño de los bloques por eje (sx,sy,sz)
    int nx;
    int ny;
    int nz;
    double sx;
    double sy;
    double sz;
};

//Declaración de las funciones de grid.cpp
std::vector<ConstantesMalla> Grid(ConstantesGlobales& constantes);

#endif // GRID_HPP
