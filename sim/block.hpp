#ifndef ARQUITECTURA_PRACTICA_BLOCK_HPP
#define ARQUITECTURA_PRACTICA_BLOCK_HPP
#include <cmath>
#include "../constantes.hpp"
#include <vector>
#include "progargs.hpp"
#include "grid.hpp"

struct Bloque {
    //La estructura bloque contiene: id; array de partículas; array de bloques adyacentes
    int id;
    std::vector<int> particula;
    std::vector<int> adyacentes;

    Bloque(int id_)
            : id(id_){
    }
};

//Declaración de las funciones en block.cpp
double ajustarLim(double valor, double LimiteInf, double LimiteSup);
double calcularDistij(double distanciaCuadrada);
void anadir_particulas(std::vector<Bloque>& bloques , std::vector<Particula>& particulas, const std::vector<ConstantesMalla>& malla_valores);
double diferenciaDistancias(std::vector<Particula>& particulas,int particula_i, int particula_j);
bool ValidarIndices(int i, int j, int k, const std::vector<ConstantesMalla>& malla_valores);
void anotar_adyacentes(std::vector<Bloque>& bloques, const std::vector<ConstantesMalla>& malla_valores);
void incrementar_densidades(std::vector<Bloque>& bloques, std::vector<Particula>& particulas, std::vector<ConstantesGlobales>& constantes);
void transformar_densidades(std::vector<Particula>& particulas, std::vector<ConstantesGlobales>& constantes);
void transferencia_aceleracion(std::vector<Bloque>& bloques, std::vector<Particula>& particulas, std::vector<ConstantesGlobales>& constantes);
void colisiones_part_x(Particula &particula, const std::vector<ConstantesMalla>& malla_valores);
void colisiones_part_y(Particula &particula,const std::vector<ConstantesMalla>& malla_valores);
void colisiones_part_z(Particula &particula,const std::vector<ConstantesMalla>& malla_valores);
void movimiento_particulas(Particula &particula);
void colisiones_lim_x(Particula &particula,const std::vector<ConstantesMalla>& malla_valores);
void colisiones_lim_y(Particula &particula,const std::vector<ConstantesMalla>& malla_valores);
void colisiones_lim_z(Particula &particula,const std::vector<ConstantesMalla>& malla_valores);
void colisiones_limites(Particula &particula, const std::vector<ConstantesMalla>& malla_valores);
void colisiones_particulas(std::vector<Particula>& particulas,const std::vector<ConstantesMalla>& malla_valores);
void eliminar_particulas(std::vector<Bloque>& bloques);
void reiniciar_particulas(std::vector<Particula>& particulas);
void crearBloques(std::vector<Bloque>& bloques,const std::vector<ConstantesMalla>& malla_valores);
//void imprimir_particulas(const std::vector<Particula>& particulas, const std::string& filename);
void guardar(const std::string& filename, const std::vector<Particula>& particulas, float ppm, int np);

#endif //ARQUITECTURA_PRACTICA_BLOCK_HPP