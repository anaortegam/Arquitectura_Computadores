
#ifndef FLUID_CONSTANTES_HPP
#define FLUID_CONSTANTES_HPP

#include <iostream>
#include <cmath>


const double radio = 1.695;
const double densidad = 1e3;
const double presion_de_rigidez = 3.0;
const double colisiones_de_rigidez = 3.0e4;
const double amortiguamiento = 128.0;
const double viscosidad = 0.4;
const double tamano_de_particula = 2.0e-4;
const double paso_de_tiempo = 1.0e-3;
const double aceleracion_x = 0.0;
const double aceleracion_y = -9.8;
const double aceleracion_z = 0.0;
const double xmin = -0.065;
const double xmax = 0.065;
const double ymin = -0.08;
const double ymax = 0.1;
const double zmin = -0.065;
const double zmax = 0.065;
const double numero_dens1= 315;
const double numero_dens2 = 64;
const double epsilon = 1e-12;
const double quince = 15;
const double m_pi = M_PI;
const double quarentaycinco = 45;
const double actualiza_aceleracion= 10e-10;
const int error_menoscinco= -5;
#endif //FLUID_CONSTANTES_HPP