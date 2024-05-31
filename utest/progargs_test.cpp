#include "gtest/gtest.h"
#include "../sim/progargs.hpp"
#include <iostream>
#include <filesystem>



//Test NOT OKEY: Cero argumentos.
TEST(SimMain, CeroArgumentos) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(255), "Error: Invalid number of arguments: 0");
}

//Test NOT OKEY: Un argumento.
TEST(SimMain, UnArgumento) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program", "200"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(255), "Error: Invalid number of arguments: 1");
}

//Test NOT OKEY: Dos argumentos.
TEST(SimMain, DosArgumentos) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program", "0", "input.fld"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(255), "Error: Invalid number of arguments: 2");
}
//Test NOT OKEY:EL número de argumentos es mayor.
TEST(SimMain, MasDeTresArgumentos) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program", "0", "input.fld", "final.fld", "45"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(255), "Error: Invalid number of arguments: 4");

}

//Test NOT OKEY: Numero de pasos no es entero.
TEST(SimMain, NumeroPasosNoEntero) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program", "hello", "input.fld", "final.fld"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(255), "Error: Time steps must be numeric.");
}

//Test NOT OKEY: Numero de pasos es negativo.
TEST(SimMain, NumeroPasosNegativo) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program", "-2", "input.fld", "final.fld"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(254), "Error: Invalid number of time steps.");
}

//Test NOT OKEY.No se puede leer el archivo de lectura.
TEST(SimMain, ArchivoDeLecturaNoLegible) {
    const std::vector<std::string> notOkeyArgs = {"./fluid_program", "2", "Error.fld", "final.fld"};
    const int tamano = static_cast<int>(notOkeyArgs.size());
    EXPECT_EXIT(sim_main(tamano, notOkeyArgs), ::testing::ExitedWithCode(253), "Error: Cannot open Error.fld for reading.");
}



//Test NOT OKEY:No existe el archivo.
TEST(LoadData, FileNotFound) {
    const std::string noExiste= "no_existefld";
    std::vector<Particula> particulas;
    float ppm= 0;
    int n_particulas= 0;
    EXPECT_FALSE(cargarDatos(noExiste, particulas, ppm, n_particulas));
}

//TEST OKEY.PROGARGS(READ PARTICLES PPM AND NP).
TEST(LoadData, ValidData) {
    const std::filesystem::path currentSourcePath = std::filesystem::path(__FILE__).parent_path().parent_path();
    const std::filesystem::path validDataFile = currentSourcePath / "small.fld";

    const float ppm = 204;
    const int validNp = 4800;

    std::vector<Particula> loadedParticles;
    float loadedPpm=0;
    int loadedNp=0;

    EXPECT_TRUE(cargarDatos(validDataFile.string(), loadedParticles, loadedPpm, loadedNp));

    EXPECT_EQ(ppm, loadedPpm);
    EXPECT_EQ(validNp, loadedNp);
}

//TEST_OKEY:Leer_Valores_Entrada.
TEST(SimMain, LecturaValoresDeEntrada) {
    std::vector<Particula> particulas;
    const std::vector<const char *> argv = {"./fluid_program", "5", "large.fld", "out.fld"};
    const std::filesystem::path currentSourcePath = std::filesystem::path(__FILE__).parent_path().parent_path();
    const std::filesystem::path validDataFile = currentSourcePath / "large.fld";
    const int num_particulas_correcto = 15138;float ppm_fluid=0;int numero_particulas=0;
    if (!cargarDatos(validDataFile, particulas, ppm_fluid, numero_particulas)) {std::cerr << "Error al cargar el archivo de entrada.\n";}
    EXPECT_EQ(num_particulas_correcto, particulas.size());
}



