#!/bin/bash


# g++ -lpthread -ffast-math -msse -lm src/* -Iinclude -lsfml-graphics -o makeitpixel
# g++ -O3 src/* -Iinclude -lsfml-graphics -o makeitpixel
g++ -ffast-math -msse src/* -Iinclude -lsfml-graphics -o makeitpixel
# time ./makeitpixel -c examples/config1.json -o doc /home/robert/Desktop/MakeItPixel/examples/monalisa.png


# time ./makeitpixel -c examples/config3.json -o doc IMG_7210.jpeg

time ./makeitpixel -c examples/config3.json -o doc test.jpg


# config1.json, config3.json 對 dither 的優化比較明顯