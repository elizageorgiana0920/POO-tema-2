cmake_minimum_required(VERSION 3.10)
project(cafenea_tema2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cafenea_tema2
        main.cpp
        Ingredient.cpp
        Produs.cpp
        Bautura.cpp
        Patiserie.cpp
        Gestiune.cpp
        Comanda.cpp
        Autentificare.cpp
        Sandwich.cpp
)