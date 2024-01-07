// #include <pthread.h>
#include "Thread.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>

void *thread_min_max(void *args)
{
    struct min_max_args *min_max_args = (struct min_max_args *) args;  
    sf::Vector2u imgSize = min_max_args->image->getSize();

    for(int y = 0; y < min_max_args->num_of_rows; y++){
        int r = min_max_args->num_of_rows * min_max_args->thread_id + y; // 從 thread id * num_of_row 開始畫
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
        int r = min_max_args->num_of_rows * min_max_args->thread_id + y; // 從 thread id * num_of_rows 開始畫
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

void *thread_pixel(void *args)
{
    struct pixel_args *pixel_args = (struct pixel_args *) args;

    sf::Vector2u origImgSize = pixel_args->image->getSize();
    std::cout << origImgSize.x << " " << origImgSize.y << std::endl;

    RGB (*selectorfun)(const Palette&);        
    if(pixel_args->selector == "avg"){
        selectorfun = [](const Palette& p)->RGB{
            uint sr=0, sg=0, sb=0, sa=0;
            for(auto& c: p){
                sr += c.r;
                sg += c.g;
                sb += c.b;
                sa += c.a;
            }
            uint n = p.size();
            return RGB(sr/n, sg/n, sb/n, sa/n);
        };
    }else if(pixel_args->selector == "med"){
        selectorfun = [](const Palette& p)->RGB{
            return graySorted(p)[p.size()/2];
        };
    }else if(pixel_args->selector == "min"){
        selectorfun = [](const Palette& p)->RGB{
            return graySorted(p)[0];
        };
    }else if(pixel_args->selector == "max"){
        selectorfun = [](const Palette& p)->RGB{
            return graySorted(p)[p.size()-1];
        };
    }else{
        throw std::runtime_error("Unkown pixel selector: "+ pixel_args->selector);
    }


    for(uint k = 0; k <= pixel_args->num_of_rows; k++){
        uint j = pixel_args->num_of_rows * pixel_args->thread_id + k;
        for(uint i = 0; i <= pixel_args->width; i++){
            std::vector<RGB> block;
            for(uint bj = 0; bj < pixel_args->blockheight; bj++){
                uint y = j * pixel_args->blockheight + bj; // 算出 block 的 y 座標
                if(y >= origImgSize.y) break;
                for(uint bi = 0; bi < pixel_args->blockwidth; bi++){
                    uint x = i * pixel_args->blockwidth + bi; // 算出 block 的 x 座標
                    if(x >= origImgSize.x) break;
                    block.push_back(pixel_args->image->getPixel(x,y));
                }
            }

            // newImage 應該要作為 critical section，看要怎麼處理，感覺比較好的方式是每個 thread 各自算，然後
            if(!block.empty()) {
                // sf::Color pixel_color;
                // pixel_color.r = 0;
                // pixel_color.g = 0;
                // pixel_color.b = 0;
                // pixel_args->newImage->setPixel(i, j, pixel_color);
                pixel_args->newImage->setPixel(i, j, selectorfun(block));
                // RGB tmp = selectorfun(block);
                // printf("r %d, b %d, g %d\n", tmp.r, tmp.b, tmp.g);
            }
        }
    }
    pthread_exit(NULL);
}
