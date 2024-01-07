#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include <SFML/Graphics.hpp>

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


void *thread_min_max(void *arg);
void *thread_normalize(void *args);
// void *thread_min_max();



#endif