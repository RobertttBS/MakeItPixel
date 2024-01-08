#!/bin/bash


# g++ -lpthread -ffast-math -msse -lm src/* -Iinclude -lsfml-graphics -o makeitpixel
# g++ -O3 src/* -Iinclude -lsfml-graphics -o makeitpixel
g++ -ffast-math -msse src/* -Iinclude -lsfml-graphics -o makeitpixel
# time ./makeitpixel -c examples/config1.json -o doc /home/robert/Desktop/MakeItPixel/examples/monalisa.png


time ./makeitpixel -c examples/config4.json -o doc IMG_7210.jpeg