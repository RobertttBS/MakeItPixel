// #include <pthread.h>
#include "Thread.hpp"
#include <stdio.h>

void *thread_min_max(void *args)
{
    struct min_max_args *min_max_args = (struct min_max_args *) args;  
    sf::Vector2u imgSize = min_max_args->image->getSize();

    for(int y = 0; y < min_max_args->num_of_rows; y++){
        // 從 thread id * num_of_row 開始畫
        int r = min_max_args->num_of_rows * min_max_args->thread_id + y;
        for(int c = 0; c < imgSize.x; c++){
            sf::Color pixel_color = min_max_args->image->getPixel(c, r);
            min_max_args->minR = std::min(min_max_args->minR, pixel_color.r);
            min_max_args->minG = std::min(min_max_args->minG, pixel_color.g);
            min_max_args->minB = std::min(min_max_args->minB, pixel_color.b);
            min_max_args->maxR = std::max(min_max_args->maxR, pixel_color.r);
            min_max_args->maxG = std::max(min_max_args->maxG, pixel_color.g);
            min_max_args->maxB = std::max(min_max_args->maxB, pixel_color.b);
        }
    }
    pthread_exit(NULL);
}

void *thread_normalize(void *args)
{
    struct min_max_args *min_max_args = (struct min_max_args *) args;  
    sf::Vector2u imgSize = min_max_args->image->getSize();
    int dr = min_max_args->maxR - min_max_args->minR;
    int dg = min_max_args->maxG - min_max_args->minG;
    int db = min_max_args->maxB - min_max_args->minB;

    for(int y = 0; y < min_max_args->num_of_rows; y++){
        // 從 thread id * num_of_row 開始畫
        int r = min_max_args->num_of_rows * min_max_args->thread_id + y;
        for(int c = 0; c < imgSize.x; c++){
            sf::Color pixel_color = min_max_args->image->getPixel(c, r);
            pixel_color.r = 255 * ((float)pixel_color.r - min_max_args->minR)/dr;
            pixel_color.g = 255 * ((float)pixel_color.g - min_max_args->minG)/dg;
            pixel_color.b = 255 * ((float)pixel_color.b - min_max_args->minB)/db;
            min_max_args->image->setPixel(c, r, pixel_color);
        }
    }
    pthread_exit(NULL);
}
