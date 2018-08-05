#!/usr/bin/env sh

INCLUDE=include
SFML_INCLUDE=C:/Libraries/sfml/include
SFML_LIB=C:/Libraries/sfml/lib

for f in src/*.cpp
do
    FILE=${f%.*}
    FILE=${FILE##*/}
    g++ -c -g -std=c++11 -Iinclude -I$SFML_INCLUDE $f -o obj/$FILE.o -Wall
done

for f in src/graphics/*.cpp
do
    FILE=${f%.*}
    FILE=${FILE##*/}
    g++ -c -g -std=c++11 -Iinclude -I$SFML_INCLUDE $f -o obj/graphics/$FILE.o -Wall
done

echo "Building..."
g++ -g -o bin/Exodus.exe obj/*.o obj/graphics/*.o -DDEBUG -I$INCLUDE -I$SFML_INCLUDE -L$SFML_LIB -lsfml-graphics -lsfml-window -lsfml-system
