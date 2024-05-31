#include <iostream>
#include <fstream>
#include <vector>
#include "progargs.hpp"


template <typename T>
char* as_writable_buffer(T & value);

template <typename T>
T read_binary_value(std::istream & is);


template <typename T>
char* as_writable_buffer_impl(T & value) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<char *>(&value);
}


template <typename T>
char* as_writable_buffer(T & value) {
    return as_writable_buffer_impl(value);
}


template <typename T>
T read_binary_value_impl(std::istream & is) {
    T value{};
    is.read(as_writable_buffer(value), sizeof(value));
    return value;
}


template <typename T>
T read_binary_value(std::istream & is) {
    return read_binary_value_impl<T>(is);
}

void cargarParticula(std::vector<Particula>& particulas, std::ifstream& file, int np, int& contador){
    //Carga las partículas del archivo a la estructura Particula
    for (int i = 0; i < np; ++i) {
        //Inicializacion de una partícula
        Particula particle{0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0,{0.0, aceleracion_y, 0.0},0, 0, 0,0};
        //Lectura de los componentes de la partícula y asignación en la partícula
        particle.px  = static_cast<double>(read_binary_value<float>(file));
        particle.py  = static_cast<double>(read_binary_value<float>(file));
        particle.pz = static_cast<double>(read_binary_value<float>(file));
        particle.hvx = static_cast<double>(read_binary_value<float>(file));
        particle.hvy = static_cast<double>(read_binary_value<float>(file));
        particle.hvz=  static_cast<double>(read_binary_value<float>(file));
        particle.vx = static_cast<double>(read_binary_value<float>(file));
        const auto  vy_float = read_binary_value<float>(file);
        const auto vz_float = read_binary_value<float>(file);

        //Conversión de los valores float a double
        particle.vy = static_cast<double>(vy_float);
        particle.vz = static_cast<double>(vz_float);
        particle.id = contador;
        ++contador;
        particulas.push_back(particle);
    }
}

bool cargarDatos(const std::string& filename, std::vector<Particula>& particulas, float& ppm, int& np) {
    // Abre el archivo de entrada (filename)
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de entrada.\n";
        return false;
    }
    //Asignación de los parámetros contenidos en el archivo
    ppm = read_binary_value<float>(file);np = read_binary_value<int>(file);
    std::cout << "Number of particles: " << np << "\n";std::cout << "Particles per meter: " << ppm << "\n";
    //Comprobación del parámetro np
    if (np == 0) {
        std::cerr << "Invalid number of particles: " << np << "\n";file.close();
        exit( error_menoscinco);
    }
    //Carga de las partículas en la estructura
    int contador = 0;
    cargarParticula(particulas, file, np, contador );
    //Comprobación de que se han cargado todas las partículas del archivo de entrada
    if (contador != np) {
        std::cerr << "Error: Number of particles mismatch. Header:" << np << ", Found:" << contador << "\n";file.close();
        return false;
    }file.close();
    return true;}

std::vector<ConstantesGlobales> parametrosSimulacion(float ppm, double &masa, double &suavidad,std::vector<ConstantesGlobales>& constantes) {
    //Cálculo de los parámetros contenidos en la estructura ConstantesGlobales
    ConstantesGlobales constante{0,0};
    masa = densidad / (ppm * ppm * ppm);
    suavidad = radio / ppm;
    std::cout << "Smoothing length: " << suavidad << "\n";
    std::cout << "Particle mass: " << masa << "\n";
    constante.suavidad =suavidad;
    constante.masa=masa;
    constantes.push_back(constante);
    return constantes;
}
using namespace std;

bool Numero(const std::string& str) {
    //Comprueba que el parámetro es un número
    if (str.empty()) {
        return false;}
    std::string::size_type inicio = 0;
    if (str[0] == '-') {
        inicio = 1;}
    for (std::string::size_type i = inicio; i < str.size(); ++i) {
        const char valor = str[i];
        if (valor < '0' || valor > '9') {
            return false;}
    }return true;}

bool Digito(const std::string& str) {
    //Transforma la cadena de caracteres a un número
    if (str.empty()) {return false;}
    const std::string::size_type inicio = 0;
    for (std::string::size_type i = inicio; i < str.size(); ++i) {
        const char valor = str[i];
        if (valor < '0' || valor > '9') {
            return false;}
    }return true;}


int sim_main(int argc, const std::vector<std::string>& argv) {
    //Comprueba los argumentos y llama al resto de las funciones del archivo

    //Comprobación de los argumentos
    if (argc == 1) {std::cerr << "Error: Invalid number of arguments: 0" << "\n";exit(-1);}
    if (argc == 2) {std::cerr << "Error: Invalid number of arguments: 1" << "\n";exit(-1);}
    if (argc == 3) {std::cerr << "Error: Invalid number of arguments: 2" << "\n";exit(-1);}
    if (argc > 4) {std::cerr << "Error: Invalid number of arguments: 4" << "\n";exit(-1);};
    if (!Numero(argv[1])) {std::cerr << "Error: Time steps must be numeric." << "\n";exit(-1);};
    //Asignación de los argumentos en variables del código
    const int numero_pasos = std::stoi(argv[1]);
    const bool is_number = Digito(argv[1]);
    if (!is_number) {std::cerr << "Error: Invalid number of time steps." << "\n";exit(-2);}
    const char* input_file = argv[2].data();
    const char* output_file = argv[3].data();
    //Apertura del archivo
    const std::ifstream input(input_file, std::ios::binary);
    if (!input) {std::cerr << "Error: Cannot open " << input_file << " for reading." << "\n";exit(-3);}
    const std::ofstream output(output_file, std::ios::binary);
    if (!output) {std::cerr << "Error: Cannot open " << output_file << " for writing." <<"\n";exit(-4);}
    return numero_pasos;
}
