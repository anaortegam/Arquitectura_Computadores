
#include <iostream>
#include <vector>
#include "../sim/progargs.hpp"
#include "../sim/grid.hpp"
#include "../sim/block.hpp"


//función main del código
int main(int argc, char* argv[]) {
    std::vector<Particula> particulas;const std::vector<std::string> args(argv, argv + argc);
    //Análisis de argumentos
    const int numero_pasos = sim_main(argc, args);
    std::vector<Bloque> bloques;std::vector<ConstantesGlobales> constantes;
    const std::string& output_file = args[3];
    float ppm_fluid = 0;double masa = 0;double suavidad = 0; int numero_particulas=0;
    if (numero_pasos >=1) {
        //Creación de los parámetros, la malla y los bloques
        const std::string& input_file = args[2];
        if (!cargarDatos(input_file, particulas, ppm_fluid, numero_particulas)) {std::cerr << "Error al cargar el archivo de entrada.\n";return 1;}
        const std::vector<ConstantesGlobales> constantes_globales = parametrosSimulacion(ppm_fluid, masa, suavidad, constantes);
        const std::vector<ConstantesMalla> malla_valores = Grid(constantes[0]);
        crearBloques(bloques, malla_valores);
        //Procesamiento de pasos de tiempo
        for (int i = 0; i < numero_pasos; i++) {
            anadir_particulas(bloques, particulas, malla_valores);
            incrementar_densidades(bloques, particulas, constantes);
            transformar_densidades(particulas, constantes);
            transferencia_aceleracion(bloques,particulas, constantes);
            colisiones_particulas(particulas, malla_valores);
            eliminar_particulas(bloques);reiniciar_particulas(particulas);}}
    guardar(output_file, particulas, ppm_fluid, numero_particulas);
    particulas.clear();constantes.clear();
    return 0;
}