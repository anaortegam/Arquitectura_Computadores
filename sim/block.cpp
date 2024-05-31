#include "block.hpp"
#include <cmath>
#include <vector>
#include <set>
#include <fstream>


double ajustarLim(double valor, double LimiteInf, double LimiteSup) {
    //Evita que las partículas se añadan fuera de la malla
    if (valor < LimiteInf) {
        return LimiteInf;
    } if (valor >= LimiteSup) {
        return LimiteSup-1;
    }
    //Si están dentro de los límites, devuelve el valor de entrada
    return valor;
}

double calcularDistij(const double distanciaCuadrada) {
    //Si la distancia es extremadamente pequeña o 0, devuelve epsilon
    const double distij = std::max(distanciaCuadrada, epsilon);
    const double distij2 = std::sqrt(distij);

    return distij2;
}

void anadir_particulas(std::vector<Bloque>& bloques , std::vector<Particula>& particulas, const std::vector<ConstantesMalla>& malla_valores) {
    //Añade las partículas a los bloques
    for (Particula &particula: particulas) {
        //Cálculo del índice del bloque
        const int pos_i = int(ajustarLim(std::floor((particula.px - xmin) / malla_valores[0].sx), 0, malla_valores[0].nx));
        const int pos_j = int(ajustarLim(std::floor((particula.py - ymin) / malla_valores[0].sy), 0, malla_valores[0].ny));
        const int pos_k = int(ajustarLim(std::floor((particula.pz - zmin) / malla_valores[0].sz), 0, malla_valores[0].nz));
        particula.i = pos_i;
        particula.j = pos_j;
        particula.z = pos_k;
        const int index = ((pos_k * malla_valores[0].ny * malla_valores[0].nx) + (pos_j * malla_valores[0].nx) + pos_i);
        bloques[index].particula.push_back(particula.id);
    }
}

double diferenciaDistancias(std::vector<Particula>& particulas,int particula_i, int particula_j) {
    //Calcula la distancia euclidiana que hay entre dos partículas
    const double distx = particulas[particula_i].px - particulas[particula_j].px;
    const double disty =particulas[particula_i].py - particulas[particula_j].py;
    const double distz = particulas[particula_i].pz - particulas[particula_j].pz;

    const double distancia = (distx * distx )+ (disty * disty )+ (distz * distz);

    return distancia;
}


bool ValidarIndices(int i, int j, int k, const std::vector<ConstantesMalla>& malla_valores) {
    //Comprueba que el valor del índice del bloque es válido
    return i >= 0 && i < malla_valores[0].nx && j >= 0 && j < malla_valores[0].ny && k >= 0 && k < malla_valores[0].nz;
}


void anotar_adyacentes(std::vector<Bloque>& bloques, const std::vector<ConstantesMalla>& malla_valores) {
    //Anota los bloques adyacentes de cada bloque en el vector adyacentes
    for (int i = 0; i < malla_valores[0].nx; ++i) {
        for (int j = 0; j < malla_valores[0].ny; ++j) {
            for (int k = 0; k < malla_valores[0].nz; ++k) {
                const int index = (k * malla_valores[0].ny * malla_valores[0].nx) + (j * malla_valores[0].nx) + i;
                //Todas las posibles posiciones de bloques adyacentes
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        for (int dk = -1; dk <= 1; ++dk) {
                            //Comprobar que es un bloque adyacente y no el mismo bloque
                            if (ValidarIndices(i + di, j + dj, k + dk, malla_valores)) {
                                const int index2 = ((k + dk) * malla_valores[0].ny * malla_valores[0].nx) + ((j + dj) * malla_valores[0].nx) + (i + di);
                                bloques[index].adyacentes.push_back(bloques[index2].id);}
                        }
                    }
                }
            }
        }
    }
}

void incrementar_densidades(std::vector<Bloque>& bloques, std::vector<Particula>& particulas, std::vector<ConstantesGlobales>& constantes) {
    //Incrementa las densidades de cada partícula
    const double suavidadh2 = constantes[0].suavidad * constantes[0].suavidad;
    for (const Bloque& bloqueAct: bloques) {
        for (const int index_particula_i : bloqueAct.particula) {
            for (const int index_vecino : bloqueAct.adyacentes) {
                const Bloque& siguiente = bloques[index_vecino];
                for (const int index_particula_j : siguiente.particula) {
                    if (index_particula_i>index_particula_j){
                        double dens_increment = 0;
                        //Comprueba que se encuentran a una distancia válida
                        const double dist = diferenciaDistancias(particulas,index_particula_i, index_particula_j);
                        if (dist<constantes[0].suavidad * constantes[0].suavidad) {
                            dens_increment = (suavidadh2 - dist) * (suavidadh2 - dist) * (suavidadh2 - dist);
                        }
                        //Aumenta las densidades de ambas partículas
                        particulas[index_particula_i].dens+= dens_increment;
                        particulas[index_particula_j].dens+= dens_increment;
                    }
                }
            }
        }
    }
}


void transformar_densidades(std::vector<Particula>& particulas, std::vector<ConstantesGlobales>& constantes){
    //Transforma las densidades
    const double suavidadh3 = constantes[0].suavidad*constantes[0].suavidad*constantes[0].suavidad;
    const double parte2 = numero_dens1/(numero_dens2*m_pi*(suavidadh3*suavidadh3*suavidadh3));
    for (Particula& particula : particulas) {
        const double parte1 = particula.dens + (suavidadh3*suavidadh3);
        particula.dens = parte1 * parte2 * constantes[0].masa;
    }
}

void transferencia_aceleracion(std::vector<Bloque>& bloques, std::vector<Particula>& particulas, std::vector<ConstantesGlobales>& constantes) {
    //Transfiere aceleración a las partículas
    const double suavidadh6 = constantes[0].suavidad*constantes[0].suavidad*constantes[0].suavidad*constantes[0].suavidad*constantes[0].suavidad*constantes[0].suavidad;
    const double fraccion2 = quarentaycinco / (m_pi * suavidadh6);const double fraccion3 = quince / (m_pi * suavidadh6);const double fraccion4 = (3 * constantes[0].masa * presion_de_rigidez) / 2;
    for (const Bloque& BloqueAct: bloques) {
        for (const int &part_i : BloqueAct.particula) {
            for (const int index_vecino : BloqueAct.adyacentes) {
                const Bloque& siguiente = bloques[index_vecino];
                for (const int &part_j : siguiente.particula) {
                    const double dist = diferenciaDistancias(particulas,particulas[part_i].id, particulas[part_j].id);
                    if (particulas[part_i].id>particulas[part_j].id && (dist<(constantes[0].suavidad * constantes[0].suavidad))) {
                        //Calcula el incremento de la aceleración para cada eje por separado
                        const double distij = calcularDistij(dist);const double fraccion1 = ((constantes[0].suavidad - distij) * (constantes[0].suavidad - distij)) / distij;
                        const double incremento_aceleracionx =
                                (((particulas[part_i].px - particulas[part_j].px) * fraccion3 * fraccion4 * fraccion1 * (particulas[part_i].dens + particulas[part_j].dens - 2 * densidad)) +
                                 ((particulas[part_j].vx - particulas[part_i].vx) * fraccion2 * viscosidad * constantes[0].masa))/ (particulas[part_i].dens * particulas[part_j].dens);
                        const double incremento_aceleraciony =
                                (((particulas[part_i].py - particulas[part_j].py) * fraccion3 * fraccion4 * fraccion1 * (particulas[part_i].dens + particulas[part_j].dens - 2 * densidad)) +
                                 ((particulas[part_j].vy - particulas[part_i].vy) * fraccion2 * viscosidad * constantes[0].masa))/(particulas[part_i].dens * particulas[part_j].dens);
                        const double incremento_aceleracionz = (((particulas[part_i].pz - particulas[part_j].pz) * fraccion3 * fraccion4 * fraccion1 * (particulas[part_i].dens + particulas[part_j].dens - 2 * densidad)) +
                                                                ((particulas[part_j].vz - particulas[part_i].vz) * fraccion2 * viscosidad * constantes[0].masa)) / (particulas[part_i].dens * particulas[part_j].dens);
                        //Aumenta las aceleraciones de ambas partículas
                        particulas[part_i].aceleracion_externa.x += incremento_aceleracionx;particulas[part_j].aceleracion_externa.x -= incremento_aceleracionx;
                        particulas[part_i].aceleracion_externa.y += incremento_aceleraciony;particulas[part_j].aceleracion_externa.y -= incremento_aceleraciony;
                        particulas[part_i].aceleracion_externa.z += incremento_aceleracionz;particulas[part_j].aceleracion_externa.z -= incremento_aceleracionz;}}}}}}

void colisiones_part_x(Particula &particula, const std::vector<ConstantesMalla>& malla_valores) {
    //Cálculo de las colisiones para el eje x
    const double posicion_x = particula.px + particula.hvx * paso_de_tiempo;
    //Comprobación de si la partícula se encuentra en el límite de la malla
    if (particula.i == 0) {
        const double difx = tamano_de_particula - (posicion_x - xmin);
        if (difx > actualiza_aceleracion) {
            particula.aceleracion_externa.x += (colisiones_de_rigidez * difx) - (amortiguamiento * particula.vx);
        }
    } else if (particula.i == malla_valores[0].nx - 1) {
        const double difx = tamano_de_particula - (xmax - posicion_x);
        if (difx > actualiza_aceleracion) {
            particula.aceleracion_externa.x -= (colisiones_de_rigidez * difx) + (amortiguamiento * particula.vx);
        }
    }
}

void colisiones_part_y(Particula &particula,const std::vector<ConstantesMalla>& malla_valores) {
    //Cálculo de las colisiones para el eje y
    const double posicion_y = particula.py + particula.hvy * paso_de_tiempo;
    //Comprobación de si la partícula se encuentra en el límite de la malla
    if (particula.j == 0) {
        const double dify = tamano_de_particula - (posicion_y - ymin);
        if (dify > actualiza_aceleracion) {
            particula.aceleracion_externa.y += (colisiones_de_rigidez * dify) - (amortiguamiento * particula.vy);
        }
    } else if (particula.j == malla_valores[0].ny - 1) {
        const double dify = tamano_de_particula - (ymax - posicion_y);
        if (dify > actualiza_aceleracion) {
            particula.aceleracion_externa.y -= (colisiones_de_rigidez * dify) + (amortiguamiento * particula.vy);
        }
    }
}

void colisiones_part_z(Particula &particula,const std::vector<ConstantesMalla>& malla_valores) {
    //Cálculo de las colisiones para el eje z
    const double posicion_z = particula.pz + particula.hvz * paso_de_tiempo;
    //Comprobación de si la partícula se encuentra en el límite de la malla
    if (particula.z == 0) {
        const double difz = tamano_de_particula - (posicion_z - zmin);
        if (difz > actualiza_aceleracion) {
            particula.aceleracion_externa.z += (colisiones_de_rigidez * difz) - (amortiguamiento * particula.vz);
        }
    } else if (particula.z == malla_valores[0].nz - 1) {
        const double difz = tamano_de_particula - (zmax - posicion_z);
        if (difz > actualiza_aceleracion) {
            particula.aceleracion_externa.z -= (colisiones_de_rigidez * difz) + (amortiguamiento * particula.vz);
        }
    }
}

void movimiento_particulas(Particula &particula){
    //Realiza el cambio de posición, velocidad, y aceleración de las partículas

    //Cambio de posición
    particula.px = particula.px + (particula.hvx*paso_de_tiempo)+(particula.aceleracion_externa.x*std::pow(paso_de_tiempo, 2));
    particula.py = particula.py + (particula.hvy*paso_de_tiempo)+(particula.aceleracion_externa.y*std::pow(paso_de_tiempo, 2));
    particula.pz = particula.pz + (particula.hvz*paso_de_tiempo)+(particula.aceleracion_externa.z*std::pow(paso_de_tiempo, 2));

    //Cambio de velocidad
    particula.vx = particula.hvx + ((particula.aceleracion_externa.x*paso_de_tiempo)/2);
    particula.vy = particula.hvy + ((particula.aceleracion_externa.y*paso_de_tiempo)/2);
    particula.vz = particula.hvz + ((particula.aceleracion_externa.z*paso_de_tiempo)/2);

    //Cambio de aceleración
    particula.hvx = particula.hvx + (particula.aceleracion_externa.x*paso_de_tiempo);
    particula.hvy = particula.hvy + (particula.aceleracion_externa.y*paso_de_tiempo);
    particula.hvz = particula.hvz + (particula.aceleracion_externa.z*paso_de_tiempo);
}

void colisiones_lim_x(Particula &particula,const std::vector<ConstantesMalla>& malla_valores) {
    //Comprueba el límite con el eje x
    double d_x = 1;
    //Comprobación de si la partícula se encuentra en el límite de la malla
    if (particula.i == 0) {
        d_x = particula.px - xmin;
    }
    if (particula.i==malla_valores[0].nx-1){
        d_x = xmax - particula.px;
    }
    if (d_x < 0) {
        if (particula.i == 0) {
            particula.px = xmin - d_x;
            particula.px = particula.px;
            //std::cout <<"adios: "<<contador<<"\n";
        }
        if (particula.i == malla_valores[0].nx-1) {
            particula.px = xmax + d_x;
            particula.px = particula.px;
            //std::cout <<"adios: "<<contador<<"\n";
        }
        particula.vx = -particula.vx;
        particula.hvx = -particula.hvx;
    }
}

void colisiones_lim_y(Particula &particula,const std::vector<ConstantesMalla>& malla_valores) {
    //Comprueba el límite con el eje y
    double d_y = 1;
    //Comprobación de si la partícula se encuentra en el límite de la malla
    if (particula.j == 0) {
        d_y = particula.py - ymin;
    }
    if (particula.j == malla_valores[0].ny-1){
        d_y = ymax - particula.py;
    }
    if (d_y < 0) {
        if (particula.j == 0) {
            particula.py = ymin - d_y;
            particula.py = particula.py;
        }
        if (particula.j== malla_valores[0].ny-1) {
            particula.py = ymax + d_y;
            particula.py = particula.py;
        }
        particula.vy = -particula.vy;
        particula.hvy = -particula.hvy;
    }
}

void colisiones_lim_z(Particula &particula,const std::vector<ConstantesMalla>& malla_valores) {
    //Comprueba el límite con el eje z
    double d_z = 1;
    //Comprobación de si la partícula se encuentra en el límite de la malla
    if (particula.z == 0) {
        d_z = particula.pz - zmin;
    }
    if (particula.z==malla_valores[0].nz-1){
        d_z = zmax - particula.pz;
    }
    if (d_z < 0) {
        if (particula.z== 0) {
            particula.pz = zmin - d_z;
            particula.pz = particula.pz;
        }
        if (particula.z == malla_valores[0].nz-1) {
            particula.pz = zmax + d_z;
            particula.pz = particula.pz;
        }
        particula.vz = -particula.vz;
        particula.hvz = -particula.hvz;
    }
}

void colisiones_limites(Particula &particula, const std::vector<ConstantesMalla>& malla_valores) {
    //Realiza las colisiones con los límites

    colisiones_lim_x(particula, malla_valores);

    colisiones_lim_y(particula, malla_valores);

    colisiones_lim_z(particula, malla_valores);
}

void colisiones_particulas(std::vector<Particula>& particulas,const std::vector<ConstantesMalla>& malla_valores) {
    //Comprueba las partículas que chocan con los límites de la malla

    for (Particula& particle : particulas){

        colisiones_part_x(particle,malla_valores);

        colisiones_part_y(particle,malla_valores);

        colisiones_part_z(particle,malla_valores);

        movimiento_particulas(particle);

        colisiones_limites(particle,malla_valores);

    }
}
/*
void imprimir_particulas(const std::vector<Particula>& particles, const std::string& filename) {
    //Escribe de los resultados en el archivo de salida (outputFile)

    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    for (const Particula& particle : particles) {
        outputFile << "ID: " << particle.id << "\n"
                   << "Posición (x, y, z): " << particle.px << ", " << particle.py << ", " << particle.pz << "\n"
                   << "Velocidad (vx, vy, vz): " << particle.vx << ", " << particle.vy << ", " << particle.vz << "\n"
                   << "Hvx, Hvy, Hvz: " << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << "\n"
                   << "Densidad: " << particle.dens<< "\n"
                   << "Aceleración (accx, accy, accz): " << particle.aceleracion_externa.x << ", " << particle.aceleracion_externa.y << ", " << particle.aceleracion_externa.z<< "\n";
    }

    outputFile.close();
}*/

void eliminar_particulas(std::vector<Bloque>& bloques) {
    //Elimina las partículas de los bloques
    for (Bloque &bloque : bloques) {
        bloque.particula.clear();
    }
}

void reiniciar_particulas(std::vector<Particula>& particulas){
    //Reinicia los valores de las partículas y las prepara para la siguiente iteración
    for (Particula& particle : particulas) {
        particle.dens = 0;
        particle.aceleracion_externa.x = aceleracion_x;
        particle.aceleracion_externa.y = aceleracion_y;
        particle.aceleracion_externa.z = aceleracion_z;
    }
}

void crearBloques(std::vector<Bloque>& bloques,const std::vector<ConstantesMalla>& malla_valores) {
    //Crea los bloques que conforman la malla
    int id_bloque = 0;
    for (int i = 0; i < malla_valores[0].nx; ++i) {
        for (int j = 0; j < malla_valores[0].ny; ++j) {
            for (int k = 0; k < malla_valores[0].nz; ++k) {
                Bloque bloque(id_bloque);
                bloque.id = id_bloque++;
                bloques.push_back(bloque);
            }
        }
    }
    //El bloque debe anotar cuáles son sus adyacentes
    anotar_adyacentes(bloques, malla_valores);
}

template <typename T>
char* as_writable_buffer_impl(T & value) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<char*>(&value);
}

template <typename T>
char* as_writable_buffer(T & value) {
    return as_writable_buffer_impl(value);
}

template <typename T>
void write_binary_value(T value, std::ostream & os) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void guardar(const std::string& filename, const std::vector<Particula>& particulas, float ppm, int np) {
    //Escribe los paráametros de las partículas en un archivo de salida

    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de salida.\n";
        return;
    }

    write_binary_value<float>(ppm, file);
    write_binary_value<int>(np, file);
    for (const auto& particula : particulas) {
        write_binary_value<float>(static_cast<float>(particula.px), file);
        write_binary_value<float>(static_cast<float>(particula.py), file);
        write_binary_value<float>(static_cast<float>(particula.pz), file);
        write_binary_value<float>(static_cast<float>(particula.hvx), file);
        write_binary_value<float>(static_cast<float>(particula.hvy), file);
        write_binary_value<float>(static_cast<float>(particula.hvz), file);
        write_binary_value<float>(static_cast<float>(particula.vx), file);
        write_binary_value<float>(static_cast<float>(particula.vy), file);
        write_binary_value<float>(static_cast<float>(particula.vz), file);
    }
    file.close();
}
