#!/usr/bin/env sh

SFML_INCLUDE=sfml/include
SFML_LIB=sfml/lib

echo "Building..."
g++ src/main.cpp -o bin/Exodus.exe -I$SFML_INCLUDE -L$SFML_LIB -lsfml-graphics -lsfml-window -lsfml-system

