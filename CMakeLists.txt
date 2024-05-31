cmake_minimum_required(VERSION 3.26)
project(fluid LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opciones de compilador
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

# Habilitar GoogleTest Library
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Habilitar GSL Library
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG v4.0.0
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

# Ejecutar clang-tidy en todo el árbol fuente (puedes habilitarlo/deshabilitarlo temporalmente según sea necesario)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.*")

# Todas las inclusiones son relativas a la raíz del árbol fuente
include_directories(PUBLIC .)

# Procesar CMake desde los directorios "sim" y "fluid"
add_subdirectory(sim)
add_subdirectory(fluid)

# Configurar las pruebas unitarias y funcionales
enable_testing()
add_subdirectory(utest)
add_subdirectory(ftest)

