
#include <iostream>
#include <fstream>
#include <vector>
#include "../sim/progargs.hpp"
#include "../sim/grid.hpp"
#include "../sim/block.hpp"
#include "gtest/gtest.h"
#include <filesystem>

template <typename T>
T read_binary_value_impl(std::istream & is) {
    T value;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    is.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

template <typename T>
T read_binary_value(std::istream & is) {
    return read_binary_value_impl<T>(is);
}

//Carga del archivo de entrada para los test funcionales
void cargarPrueba(const std::string& filename, std::vector<Particula>& particles, float& ppm, int& np) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {std::cerr << "Error al abrir el archivo de entrada.\n";return;}
    ppm = read_binary_value<float>(file);
    np = read_binary_value<int>(file);
    //Inicialización de las partículas
    for (int i = 0; i < np; ++i) {
        Particula particle{0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0,{0.0, aceleracion_y, 0.0},0, 0, 0,0};
        particle.px  = static_cast<double>(read_binary_value<float>(file));
        particle.py  = static_cast<double>(read_binary_value<float>(file));
        particle.pz = static_cast<double>(read_binary_value<float>(file));
        particle.hvx = static_cast<double>(read_binary_value<float>(file));
        particle.hvy = static_cast<double>(read_binary_value<float>(file));
        particle.hvz=  static_cast<double>(read_binary_value<float>(file));
        particle.vx = static_cast<double>(read_binary_value<float>(file));
        const auto  vy_float = read_binary_value<float>(file);
        const auto vz_float = read_binary_value<float>(file);
        particle.vy = static_cast<double>(vy_float);
        particle.vz = static_cast<double>(vz_float);
        particles.push_back(particle);}
    file.close();
}

//Comparación de dos partículas entre dos estructuras de partículas
bool compararParticulas(const std::vector<Particula>& particula1, const std::vector<Particula>& particula2) {
    //Comprobación de que hay el mismo número de partículas en las dos estructuras
    if (particula1.size() != particula2.size()) {
        return false;
    }
    //Comprobación del id de las partículas en cada estructura
    for (size_t i = 0; i < particula1.size(); ++i) {
        if (particula1[i].id != particula2[i].id) {
            return false;
        }
    }
    return true;
}

//TEST OK. El programa obtiene correctamente las trazas del archivo large.fld para 5 iteraciones.
TEST(SimulationTest, TestFuncionalLarge) {
    std::vector<Particula> particulas;
    std::vector<Bloque> bloques;
    std::vector<ConstantesGlobales> constantes;
    float ppm_fluid = 0; int numero_particulas=0;
    const std::vector<const char *> argv = {"./fluid_program", "5", "large.fld", "test_funcional.fld"};
    const std::filesystem::path currentSourcePath = std::filesystem::path(__FILE__).parent_path().parent_path();
    const std::filesystem::path validDataFile = currentSourcePath / "large.fld";
    const std::filesystem::path output_file = currentSourcePath / "test_funcional.fld";
    const std::vector<std::string> args(argv.begin(), argv.end());
    const int numero_pasos = std::stoi(argv[1]);std::cout << "numero de pasos: " << numero_pasos << "\n";
    double masa = 0;double suavidad= 0;
    if (numero_pasos >=1) {
        if (!cargarDatos(validDataFile, particulas, ppm_fluid, numero_particulas)) {std::cerr << "Error al cargar el archivo de entrada.\n";}
        const std::vector<ConstantesGlobales> constantes_globales = parametrosSimulacion(ppm_fluid, masa, suavidad, constantes);
        const std::vector<ConstantesMalla> malla_valores = Grid(constantes[0]);
        crearBloques(bloques, malla_valores);
        for (int i = 0; i < numero_pasos; i++) {
            anadir_particulas(bloques, particulas, malla_valores);
            incrementar_densidades(bloques, particulas, constantes);
            transformar_densidades(particulas, constantes);
            transferencia_aceleracion(bloques, particulas, constantes);
            colisiones_particulas(particulas, malla_valores);
            eliminar_particulas(bloques);reiniciar_particulas(particulas);}}
    guardar(output_file, particulas,ppm_fluid,numero_particulas);
    particulas.clear();constantes.clear();
    const std::filesystem::path trazafichero = currentSourcePath / "boundint-base-5_sorted_large.trz";
    std::vector<Particula> particulas1;
    float ppm_prueba = 0;float ppm_prueba2=0;
    int np_prueba = 0;int np_prueba2=0;
    cargarPrueba(output_file,particulas1,ppm_prueba,np_prueba);
    std::vector<Particula> particulas2;
    cargarPrueba(trazafichero,particulas2,ppm_prueba2,np_prueba2);
    ASSERT_TRUE(compararParticulas(particulas1, particulas2));
}

//TEST OK. El programa obtiene correctamente las trazas del archivo small.fld para 5 iteraciones.
TEST(SimulationTest, TestFuncionalSmall) {
    std::vector<Particula> particulas;
    std::vector<Bloque> bloques;
    std::vector<ConstantesGlobales> constantes;
    float ppm_fluid = 0; int numero_particulas=0;
    const std::vector<const char *> argv = {"./fluid_program", "5", "small.fld", "test_funcional_small.fld"};
    const std::filesystem::path currentSourcePath = std::filesystem::path(__FILE__).parent_path().parent_path();
    const std::filesystem::path validDataFile = currentSourcePath / "small.fld";
    const std::filesystem::path output_file = currentSourcePath / "test_funcional_small.fld";
    const std::vector<std::string> args(argv.begin(), argv.end());
    const int numero_pasos = std::stoi(argv[1]);std::cout << "numero de pasos: " << numero_pasos << "\n";
    double masa = 0;double suavidad= 0;
    if (numero_pasos >=1) {
        if (!cargarDatos(validDataFile, particulas, ppm_fluid, numero_particulas)) {std::cerr << "Error al cargar el archivo de entrada.\n";}
        const std::vector<ConstantesGlobales> constantes_globales = parametrosSimulacion(ppm_fluid, masa, suavidad, constantes);
        const std::vector<ConstantesMalla> malla_valores = Grid(constantes[0]);
        crearBloques(bloques, malla_valores);
        for (int i = 0; i < numero_pasos; i++) {
            anadir_particulas(bloques, particulas, malla_valores);
            incrementar_densidades(bloques, particulas, constantes);
            transformar_densidades(particulas, constantes);
            transferencia_aceleracion(bloques, particulas, constantes);
            colisiones_particulas(particulas, malla_valores);
            eliminar_particulas(bloques);reiniciar_particulas(particulas);}}
    guardar(output_file, particulas,ppm_fluid,numero_particulas);
    particulas.clear();constantes.clear();
    const std::filesystem::path trazafichero = currentSourcePath / "boundint-base-5_sorted_small.trz";
    std::vector<Particula> particulas1;
    float ppm_prueba = 0;float ppm_prueba2=0;
    int np_prueba = 0;int np_prueba2=0;
    cargarPrueba(output_file,particulas1,ppm_prueba,np_prueba);
    std::vector<Particula> particulas2;
    cargarPrueba(trazafichero,particulas2,ppm_prueba2,np_prueba2);
    ASSERT_TRUE(compararParticulas(particulas1, particulas2));
}

