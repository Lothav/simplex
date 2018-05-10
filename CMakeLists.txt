cmake_minimum_required (VERSION 2.8.11)
project (Simplex)
# add_definitions(-DDEBUG)
add_executable (simplex
        src/main.cpp
        src/Simplex/File.cpp
        src/Simplex/File.hpp
        src/Simplex/Tableaux.cpp
        src/Simplex/Tableaux.hpp
        src/Simplex/Matrix.cpp
        src/Simplex/Matrix.hpp
        src/Simplex/Fraction.cpp
        src/Simplex/Fraction.hpp
        external/Inifity.hpp
        )