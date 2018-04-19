cmake_minimum_required (VERSION 2.8.11)
project (Simplex)
# add_definitions(-DDEBUG)
add_executable (simplex main.c matrix.h file.h file.c util.h util.c tableaux.h tableaux.c)