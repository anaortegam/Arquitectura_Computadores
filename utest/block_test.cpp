#include "gtest/gtest.h"
#include "../sim/block.hpp"
#include "constantes_pruebas.hpp"



//TEST OKEY. Numero de bloques son los correctos.
TEST(CrearBloques, NumeroBloquesCorrecto) {
    std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{nx_prueba, ny_prueba, nz_prueba,0,0,0}};

    std::vector<Bloque> bloques;
    crearBloques(bloques, malla_valores);

    const int expectedNumBlocks = malla_valores[0].nx * malla_valores[0].ny * malla_valores[0].nz;
    ASSERT_EQ(bloques.size(), expectedNumBlocks);
}

//TEST NOT OKEY CrearBloques. Si el numero que se crea en ConstantesMalla es erróneo,no se crearn los bloques adecuados.
TEST(CrearBloques, NumeroBloquesIncorrectos) {
    ConstantesGlobales constant = {masa_prueba,0};
    const std::vector<ConstantesMalla> malla_valores1 = Grid(constant);
    const ConstantesMalla malla_valores2 = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    std::vector<Bloque> bloques;
    crearBloques(bloques, malla_valores1);
    const int expectedNumBlocks = malla_valores2.nx * malla_valores2.ny * malla_valores2.nz;
    EXPECT_NE(bloques.size(), expectedNumBlocks);
}

//TEST OKEY AnotarAdyacentes.Si se encuentra en el limite tendra 8(con el mismo) y sino como maximo 27 adyacentes.
TEST(AnotarAdyacentes, NumeroCorrectoDeAdyacentes) {
    std::vector<Bloque> bloques;
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    crearBloques(bloques, malla_valores);
    for (const Bloque& bloque_actual : bloques) {
        EXPECT_GE(bloque_actual.adyacentes.size(), 7);
        EXPECT_LE(bloque_actual.adyacentes.size(), 27);
    }
}

//TEST NOT OKEY. Más de 27 adyacentes.
TEST(AnotarAdyacentes, MasDe27Adyacentes) {
    std::vector<Bloque> bloques;
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};

    crearBloques(bloques, malla_valores);
    bloques[ejemplo_bloque].adyacentes.push_back(ejemplo_bloque2);
    for (const Bloque& bloque_actual : bloques) {
        if (bloque_actual.id == ejemplo_bloque) {
            EXPECT_FALSE(bloque_actual.adyacentes.size() >= 7 && bloque_actual.adyacentes.size() <= 27);
        }
    }
}

//TEST NOT OKEY. Menos de 8 adyacentes,quitandole sus adyacentes.
TEST(AnotarAdyacentes, MenosDe8Adyacentes) {
    std::vector<Bloque> bloques;
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};

    crearBloques(bloques, malla_valores);
    bloques[ejemplo_bloque3].adyacentes.clear();
    for (const Bloque& bloque_actual : bloques) {
        if (bloque_actual.id == ejemplo_bloque3) {
            EXPECT_FALSE(bloque_actual.adyacentes.size() >= 7 && bloque_actual.adyacentes.size() <= 27);
        }
    }
}

//TEST OKEY: Añadir una particula que pertenezca a un bloque determinado.
TEST(AnadirParticulas, AnadirUnaParticula) {
    std::vector<Bloque> bloques;
    std::vector<Particula> particulas;
    const Particula particula1 = { xmin, ymin,zmin, 0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 0, 0, 0, 0};
    particulas.push_back(particula1);
    ConstantesGlobales constantes = {suavidad_prueba,0};
    const std::vector<ConstantesMalla> malla_valores = Grid(constantes);
    crearBloques(bloques, malla_valores);
    anadir_particulas(bloques, particulas, malla_valores);
    EXPECT_EQ(bloques[0].particula.size(), 1);
}

//TEST NOT OKEY:La particula que se agrega no está dentro de los limites de la malla, no se ajustan sus valores correctamente.
TEST(AnadirParticulas, ParticulaFueraDeLaMalla) {
    std::vector<Bloque> bloques;
    std::vector<Particula> particulas;
    Particula particula1 = { xmin, ymin,zmin, 0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 0, 0, 0, 0};
    particulas.push_back(particula1);
    ConstantesGlobales constantes = {suavidad_prueba,0};
    const std::vector<ConstantesMalla> malla_valores = Grid(constantes);
    crearBloques(bloques, malla_valores);
    anadir_particulas(bloques, particulas, malla_valores);
    particula1.i = int(xmin-1);
    particula1.j = int(ymin-1);
    particula1.z = int(zmin-1);
    const int index = ((particula1.z * malla_valores[0].ny * malla_valores[0].nx) +
                       (particula1.j * malla_valores[0].nx) + particula1.i);
    EXPECT_EQ(index < (malla_valores[0].nx*malla_valores[0].ny*malla_valores[0].nz) && index >= 0, false);
}

//TEST OKEY.Ajusta los limites por abajo. Lo mismo pasaría con j y z.
TEST(AjustarLimites, AjustarLimiteInferior){
    const double valor_i = -valor_iprueba;
    const double limiteSuperior_i = valor_jprueba;
    const double limiteInferior = 0;
    EXPECT_DOUBLE_EQ(ajustarLim(valor_i, limiteInferior, limiteSuperior_i), limiteInferior);

}

//TEST OKEY.Ajusta los limites por arriba.Lo mismo pasaría con j y z.
TEST(AjustarLimites, AjustarLimiteSuperior){
    const double valor_i = valor_iprueba;
    const double limiteSuperior_i = valor_jprueba ;
    const double limiteInferior = 0;
    EXPECT_DOUBLE_EQ(ajustarLim(valor_i, limiteInferior, limiteSuperior_i), limiteSuperior_i -1);
}

//TEST OKEY:Funcion DiferenciaDistancias
TEST(DiferenciaDistancias, CalcularDiferenciaDistancias) {
    std::vector<Particula> particulas;
    const Particula particula1 = { xmax, ymax,zmax, 0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 0, 0, 0, 0};
    const Particula particula2 = { 0, 0,0, 0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 1, 0, 0, 0};
    particulas.push_back(particula1);
    particulas.push_back(particula2);
    EXPECT_EQ(diferenciaDistancias(particulas, particula1.id, particula2.id), 0.065*0.065*2+0.1*0.1);
}

//TEST OKEY:Funcion CalcularDistij, ejemplo especifico,calculo correcto.
TEST(Distanciasij, CalcularDistanciasij) {
    std::vector<Particula> particulas;
    const Particula particula1 = {0.00120396,-0.0703566,0.063208,0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 0, 0, 0, 0};
    const Particula particula2{0.00738822,-0.0713103,0.0548735 ,0, 0, 0, 0, 0, 0, 0,{0,0,0}, 1, 0, 0, 0};
    particulas.push_back(particula1);
    particulas.push_back(particula2);
    const double resultado = diferenciaDistancias(particulas, particula1.id, particula2.id); //0,33312064983
    const double res = calcularDistij(resultado);//0.5772404163
    const double epsi = epsilon; //10e-3
    EXPECT_EQ(res, std::sqrt(std::max(resultado, epsi)));  //0.5772404163>0.031622776
}

//TEST OKEY. Incrementar densidades
TEST(IncrementarDensidades, IncrementarDensidadesCorrectamente){
    std::vector<Bloque> bloques;
    std::vector<Particula> particulas;
    std::vector<ConstantesGlobales> constantes;
    const Particula particle1{  0.0388916,-0.0822749,-0.0491685,0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 0, 0, 0, 0};
    const Particula particle2{  0.0353615,-0.0823665,-0.0519613,0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    crearBloques(bloques, malla_valores);
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    bloques[0].particula.push_back(0);
    bloques[0].particula.push_back(1);
    constantes.push_back(ConstantesGlobales{suavidad_prueba, masa_prueba});
    incrementar_densidades(bloques, particulas, constantes);
    EXPECT_EQ(particulas[0].dens, 1.1597696638633993e-13);
    EXPECT_EQ(particulas[1].dens, 1.1597696638633993e-13);
}

//TEST OKEY. Transformar densidades
TEST(TransformarDensidades, TransformarDensidadesCorrectamente) {
    std::vector<Particula> particulas;
    std::vector <ConstantesGlobales> constantes;
    const ConstantesGlobales constante= {0.00830882,0.00011779};
    constantes.push_back(constante);
    const Particula particula = { xmax, ymax,zmax, 0, 0, 0, 0, 0, 0, 0,{0,aceleracion_y,0}, 0, 0, 0, 0};
    particulas.push_back(particula);
    transformar_densidades(particulas, constantes);
    EXPECT_EQ(std::floor(particulas[0].dens), std::floor(321.715));
}

//TEST OKEY. Transferencia aceleración
TEST(TransferenciaAceleracion, TransferenciaAceleracionCorrectamente) {
    std::vector<Bloque> bloques;
    std::vector<Particula> particulas;
    std::vector<ConstantesGlobales> constantes;
    constantes.push_back(ConstantesGlobales{suavidad_prueba, masa_prueba});
    const Particula particula1 = {0.0388916,-0.0822749,-0.0491685,0, 0, 0, 1, 2, 3, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const Particula particula2{0.0353615,-0.0823665,-0.0519613 ,0, 0, 0, 1, 0, 0, 1,{0,aceleracion_y,0}, 0, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    crearBloques(bloques, malla_valores);
    particulas.push_back(particula1);
    particulas.push_back(particula2);
    anadir_particulas(bloques,particulas,malla_valores);
    transferencia_aceleracion(bloques, particulas, constantes);
    EXPECT_EQ(particulas[0].aceleracion_externa.x,  -27898247932.610107);
    EXPECT_EQ(particulas[0].aceleracion_externa.y,  -656168743129.94556);
    EXPECT_EQ(particulas[0].aceleracion_externa.z,  -1005238641311.2635);
    EXPECT_EQ(particulas[1].aceleracion_externa.x, 27898247932.610107);
    EXPECT_EQ(particulas[1].aceleracion_externa.y, 656168743110.34546);
    EXPECT_EQ(particulas[1].aceleracion_externa.z, 1005238641311.2635);
}

//TEST OKEY. Colisiones eje x
TEST(ColisionesParticula, ColisionesParticulaX) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.069038, -0.0776098, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_part_x(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].aceleracion_externa.x, 137.57763599999981);
}

TEST(ColisionesParticula, ColisionesParticulaNX) {
    std::vector<Particula> particulas;
    const Particula particula = {0.069038, -0.0776098, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 14, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_part_x(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].aceleracion_externa.x, -116.70236400000016);
}

TEST(ColisionesParticula, ColisionesParticulaY) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0876098, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_part_y(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].aceleracion_externa.y, 208.24156959999979);
}

TEST(ColisionesParticula, ColisionesParticulaNY) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, 0.10876098, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 20, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_part_y(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].aceleracion_externa.y, -294.88183039999984);
}

TEST(ColisionesParticula, ColisionesParticulaZ) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0776098, -0.069038, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_part_z(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].aceleracion_externa.z, 137.44504640000017);
}

TEST(ColisionesParticula, ColisionesParticulaNZ) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0776098, 0.069038, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 14};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_part_z(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].aceleracion_externa.z, -116.83495359999979);
}

TEST(MovimientoParticulas, ActualizaParticulasCorrectamente) {
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0776098, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1143.8,{-161.677, -187.537, -150.738}, 1, 0, 0, 0};
    particulas.push_back(particula);
    movimiento_particulas(particulas[0]);

    EXPECT_FLOAT_EQ(particulas[0].px, -0.054197028);
    EXPECT_FLOAT_EQ(particulas[0].py, -0.07761851);
    EXPECT_FLOAT_EQ(particulas[0].pz, -0.054339118);
    EXPECT_FLOAT_EQ(particulas[0].vx, -0.2123885);
    EXPECT_FLOAT_EQ(particulas[0].vy, 0.085059501);
    EXPECT_FLOAT_EQ(particulas[0].vz, -0.201649);
    EXPECT_FLOAT_EQ(particulas[0].hvx, -0.29322699);
    EXPECT_FLOAT_EQ(particulas[0].hvy, -0.0087090004);
    EXPECT_FLOAT_EQ(particulas[0].hvz, -0.27701801);
}

TEST(ColisionesLimite, ColisionesLimiteX) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.069038, -0.0776098, -0.069038, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_lim_x(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].px, -0.060962);
    EXPECT_EQ(particulas[0].vx, 0.050712);
    EXPECT_EQ(particulas[0].hvx, 0.13155);
}

TEST(ColisionesLimite, ColisionesLimiteNX) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {0.069038, -0.0776098, -0.069038, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 14, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_lim_x(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].px, 0.060962);
    EXPECT_EQ(particulas[0].vx, 0.050712);
    EXPECT_EQ(particulas[0].hvx, 0.13155);
}

TEST(ColisionesLimite, ColisionesLimiteY) {
    //Particula 0 large.
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0876098, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_lim_y(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].py, -0.0723902);
    EXPECT_EQ(particulas[0].vy, -0.0850593);
    EXPECT_EQ(particulas[0].hvy, -0.178828);
}

TEST(ColisionesLimite, ColisionesLimiteNY) {
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, 0.1555, -0.0540621, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 20, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_lim_y(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].py, 0.044500000000000012);
    EXPECT_EQ(particulas[0].vy, -0.0850593);
    EXPECT_EQ(particulas[0].hvy, -0.178828);
}

TEST(ColisionesLimite, ColisionesLimiteZ) {
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0776098, -0.069038, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 0};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_lim_z(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].pz, -0.060962);
    EXPECT_EQ(particulas[0].vz, 0.0509113);
    EXPECT_EQ(particulas[0].hvz, 0.12628);
}

TEST(ColisionesLimite, ColisionesLimiteNZ) {
    std::vector<Particula> particulas;
    const Particula particula = {-0.0539038, -0.0776098, 0.069038, -0.13155, 0.178828, -0.12628, -0.050712, 0.0850593, -0.0509113, 1,{0,aceleracion_y,0}, 1, 0, 0, 14};
    const std::vector<ConstantesMalla> malla_valores = {ConstantesMalla{15, 21, 15, 0, 0, 0}};
    particulas.push_back(particula);
    colisiones_lim_z(particulas[0], malla_valores);

    EXPECT_EQ(particulas[0].pz, 0.060962);
    EXPECT_EQ(particulas[0].vz, 0.0509113);
    EXPECT_EQ(particulas[0].hvz, 0.12628);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
