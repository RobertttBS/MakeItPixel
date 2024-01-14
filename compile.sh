#!/bin/bash


g++ -lpthread src/* -Iinclude -lsfml-graphics -o makeitpixel

time ./makeitpixel -c examples/config3.json -o doc test.jpg

# config1.json, config3.json 對 dither 的優化比較明顯
