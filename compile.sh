#!/bin/bash


# g++ -lpthread -ffast-math -msse -lm src/* -Iinclude -lsfml-graphics -o makeitpixel
# g++ -O3 src/* -Iinclude -lsfml-graphics -o makeitpixel
g++ -ffast-math -msse src/* -Iinclude -lsfml-graphics -o makeitpixel
time ./makeitpixel -c examples/config3.json -o doc IMG_7829.jpeg