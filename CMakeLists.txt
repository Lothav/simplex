cmake_minimum_required (VERSION 2.8.11)
project (Simplex)
# add_definitions(-DDEBUG)
add_executable (simplex
        src/main.cpp
        src/File.cpp
        src/File.hpp
        src/Tableaux.cpp
        src/Tableaux.hpp
        src/Matrix.cpp
        src/Matrix.hpp
        src/Fraction.cpp
        src/Fraction.hpp
        external/Inifity.hpp
        )