cmake_minimum_required (VERSION 2.8.11)
project (Simplex)
# add_definitions(-DDEBUG)
add_executable (simplex main.cpp Matrix.cpp Matrix.hpp Fraction.hpp Fraction.cpp Tableaux.cpp Tableaux.hpp File.cpp File.hpp)