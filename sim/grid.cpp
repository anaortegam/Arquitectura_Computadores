#include "grid.hpp"
#include <cmath>



std::vector<ConstantesMalla> Grid(ConstantesGlobales& constantes) {
    //Genera la malla
    std::vector<ConstantesMalla> const_malla;
    ConstantesMalla constante_malla{0, 0, 0, 0, 0, 0};
    //Cálculo del número de bloques por eje para cubrir la malla
    constante_malla.nx = std::floor((xmax - xmin) / constantes.suavidad);
    const int nx_valor = std::floor((xmax - xmin) / constantes.suavidad);
    constante_malla.ny = std::floor((ymax - ymin) / constantes.suavidad);
    const int ny_valor = std::floor((ymax - ymin) / constantes.suavidad);
    constante_malla.nz = std::floor((zmax - zmin) / constantes.suavidad);
    const int nz_valor = std::floor((zmax - zmin) / constantes.suavidad);
    //Cálculo del tamaño de los bloques por eje
    constante_malla.sx = (xmax - xmin) / nx_valor;
    constante_malla.sy =(ymax - ymin)/ ny_valor;
    constante_malla.sz = (zmax - zmin)/nz_valor;
    const_malla.push_back(constante_malla);
    //Impresión del tamaño de la malla, el número de bloques y el tamaño de los bloques
    std::cout<<"Grid size: "<<nx_valor<<"x"<<ny_valor<<"x"<<nz_valor<<"\n";
    std::cout<<"Number of blocks: "<< nx_valor*ny_valor*nz_valor<<"\n";
    std::cout<<"Block size: "<<constante_malla.sx<<"x"<<constante_malla.sy<<"x"<<constante_malla.sz<<"\n";
    return const_malla;
}
