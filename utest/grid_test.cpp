#include "gtest/gtest.h"
#include "../sim/grid.hpp"
#include "constantes_pruebas.hpp"

//Test OKEY. Valores nx,ny,nz de la malla son correctos.
TEST(Malla, CalculoTamanoDeMallaCorrecto){
    ConstantesGlobales constantes = {suavidad_prueba ,0};
    std::vector<ConstantesMalla> malla_valores = Grid(constantes);

    EXPECT_EQ(malla_valores[0].nx, 15);
    EXPECT_EQ(malla_valores[0].ny, 21);
    EXPECT_EQ(malla_valores[0].nz, 15);

}
//Test OKEY. Valores sx,sy,sz de los bloques que están dentro de la malla son correctos.
TEST(Malla, CalculoNumeroDeBloquesCorrecto) {
    ConstantesGlobales constantes = {suavidad_prueba,0};
    const std::vector<ConstantesMalla> malla_valores = Grid(constantes);
    EXPECT_FLOAT_EQ(malla_valores[0].sx, 0.00866667);
    EXPECT_FLOAT_EQ(malla_valores[0].sy, 0.00857143);
    EXPECT_FLOAT_EQ(malla_valores[0].sz, 0.00866667);
}
//Test NOT OKEY: que sea un valor distinto(NX,NY,NZ). Ejemplo con 1.
TEST(Malla, CalculoTamanoDeMallaIncorrecto){
    ConstantesGlobales constantes = {suavidad_prueba,0};
    const std::vector<ConstantesMalla> malla_valores = Grid(constantes);
    EXPECT_NE(malla_valores[0].nx, 1);
    EXPECT_NE(malla_valores[0].ny, 1);
    EXPECT_NE(malla_valores[0].nz, 1);
}

//Test NOT OKEY: que sea un valor distinto(SX,SY,SZ). Ejemplo con 1.
TEST(Malla, CalculoNumeroDeBloquesIncorrecto){
    ConstantesGlobales constantes = {suavidad_prueba,0};
    const std::vector<ConstantesMalla> malla_valores = Grid(constantes);
    EXPECT_NE(malla_valores[0].sx, 1);
    EXPECT_NE(malla_valores[0].sy, 1);
    EXPECT_NE(malla_valores[0].sz, 1);
}



