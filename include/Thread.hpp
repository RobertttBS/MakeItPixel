#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include "Palette.hpp"
#include <string>

using namespace mipa;

struct min_max_args {
    int thread_id;
    int num_of_rows;
    sf::Image *image; // 如果這裡要用 reference，就要寫個 constructor 之類的
    sf::Uint8 minR;
    sf::Uint8 minG;
    sf::Uint8 minB;
    sf::Uint8 maxR;
    sf::Uint8 maxG;
    sf::Uint8 maxB;
};

struct pixel_args {
    int thread_id;
    int num_of_rows;
    uint height;
    uint width;
    uint blockheight;
    uint blockwidth;
    const sf::Image *image;
    sf::Image *newImage;
    std::string selector;
};


void *thread_min_max(void *arg);
void *thread_normalize(void *args);
void *thread_pixel(void *args);



#endif