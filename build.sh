#!/usr/bin/env sh

INCLUDE=include
SFML_INCLUDE=sfml/include
SFML_LIB=sfml/lib

echo "Building..."
g++ src/main.cpp -g -o bin/Exodus.exe -DDEBUG -I$INCLUDE -I$SFML_INCLUDE -L$SFML_LIB -lsfml-graphics -lsfml-window -lsfml-system

