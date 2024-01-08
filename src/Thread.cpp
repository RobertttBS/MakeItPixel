// #include <pthread.h>
#include "Thread.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>

void *thread_min_max(void *args)
{
    struct min_max_args *min_max_args = (struct min_max_args *) args;  
    sf::Vector2u imgSize = min_max_args->image->getSize();

    for (int y = 0; y < min_max_args->num_of_rows; y++) {
        int r = min_max_args->num_of_rows * min_max_args->thread_id + y; // 從 thread id * num_of_row 開始畫
        for (int c = 0; c < imgSize.x; c++) {
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

    for (int y = 0; y < min_max_args->num_of_rows; y++) {
        int r = min_max_args->num_of_rows * min_max_args->thread_id + y; // 從 thread id * num_of_rows 開始畫
        for (int c = 0; c < imgSize.x; c++) {
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
    RGB (*selectorfun) (const Palette&);
      
    // 把 selector function 設定好
    if (pixel_args->selector == "avg") {
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
    } else if (pixel_args->selector == "med") {
        selectorfun = [](const Palette& p)->RGB{
            return graySorted(p)[p.size()/2];
        };
    } else if (pixel_args->selector == "min") {
        selectorfun = [](const Palette& p)->RGB{
            return graySorted(p)[0];
        };
    } else if (pixel_args->selector == "max") {
        selectorfun = [](const Palette& p)->RGB{
            return graySorted(p)[p.size()-1];
        };
    } else {
        throw std::runtime_error("Unkown pixel selector: "+ pixel_args->selector);
    }

    for (uint k = 0; k <= pixel_args->num_of_rows; k++) {
        uint j = pixel_args->num_of_rows * pixel_args->thread_id + k;
        for (uint i = 0; i <= pixel_args->width; i++) {
            std::vector<RGB> block;
            for (uint bj = 0; bj < pixel_args->blockheight; bj++) {
                uint y = j * pixel_args->blockheight + bj; // 算出 block 的 y 座標
                if(y >= origImgSize.y) break;
                for (uint bi = 0; bi < pixel_args->blockwidth; bi++) {
                    uint x = i * pixel_args->blockwidth + bi; // 算出 block 的 x 座標
                    if(x >= origImgSize.x) break;
                    block.push_back(pixel_args->image->getPixel(x,y));
                }
            }

            // 不同的 width, height 會讓下面這個部分出錯
            if (!block.empty()) {
                pixel_args->newImage->setPixel(i, j, selectorfun(block));
            }
        }
    }
    pthread_exit(NULL);
}


void threadDirectQuantize(sf::Image& image, std::function<RGB(const RGB&)> &quant) {
    sf::Vector2u imgSize = image.getSize();
    pthread_t threads[THREAD_NUM];
    struct dither_args dither_args[THREAD_NUM];
    int num_of_rows = (imgSize.y + THREAD_NUM - 1) / THREAD_NUM;

    for (int i = 0; i < THREAD_NUM; i++) {
        dither_args[i].thread_id = i;
        dither_args[i].num_of_rows = num_of_rows;
        dither_args[i].image = &image;
        dither_args[i].quant = quant;
        if (pthread_create(&threads[i], NULL, thread_direct_quantize, (void *) &dither_args[i]) != 0) {
            printf("Thread creation failed\n");
            exit(-1);
        }
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Thread join failed\n");
            exit(-1);
        }
    }
}


void *thread_direct_quantize(void *args)
{
    struct dither_args *dither_args = (struct dither_args *) args;
    sf::Vector2u imgSize = dither_args->image->getSize();

    for (int y = 0; y < dither_args->num_of_rows; y++) {
        int r = dither_args->num_of_rows * dither_args->thread_id + y; // 從 thread id * num_of_row 開始畫
        for (int c = 0; c < imgSize.x; c++) {
            dither_args->image->setPixel(c, r, dither_args->quant(dither_args->image->getPixel(c, r)));
        }
    }
    pthread_exit(NULL);
}

void threadDitherOrdered(sf::Image& image, std::function<RGB(const RGB&)> &quant, const Matrix& m, double sparsity, float threshold)
{
    sf::Vector2u imgSize = image.getSize();
    pthread_t threads[THREAD_NUM];
    struct dither_args dither_args[THREAD_NUM];
    int num_of_rows = (imgSize.y + THREAD_NUM - 1) / THREAD_NUM;

    for (int i = 0; i < THREAD_NUM; i++) {
        dither_args[i].thread_id = i;
        dither_args[i].num_of_rows = num_of_rows;
        dither_args[i].image = &image;
        dither_args[i].quant = quant;
        dither_args[i].m = m;
        dither_args[i].sparsity = sparsity;
        dither_args[i].threshold = threshold;
        if (pthread_create(&threads[i], NULL, thread_dither_ordered, (void *) &dither_args[i]) != 0) {
            printf("Thread creation failed\n");
            exit(-1);
        }
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Thread join failed\n");
            exit(-1);
        }
    }
}

void *thread_dither_ordered(void *args)
{
    struct dither_args *dither_args = (struct dither_args *) args;
    double N = dither_args->m.getHeight() * dither_args->m.getWidth();
    sf::Vector2u imgSize = dither_args->image->getSize();

    for(uint k = 0; k < dither_args->num_of_rows; k++) {
        uint y = dither_args->num_of_rows * dither_args->thread_id + k; // 從 thread id * num_of_row 開始畫
        for(uint x = 0; x < imgSize.x; x++){
            RGB oldColor = dither_args->image->getPixel(x,y);
            double mij = dither_args->m.get(y % dither_args->m.getHeight(), x % dither_args->m.getWidth()) / N - 0.5;
            auto clamp = [](int x)->int{return std::min(255,std::max(0,x));};
            RGB interColor;
            interColor.r = clamp((double)oldColor.r + dither_args->sparsity * mij);
            interColor.g = clamp((double)oldColor.g + dither_args->sparsity * mij);
            interColor.b = clamp((double)oldColor.b + dither_args->sparsity * mij);
            RGB newColor = dither_args->quant(interColor);
            newColor.a = oldColor.a;
            RGB quantOldColor = dither_args->quant(oldColor);
            float err = rgbDistance(oldColor, newColor);
            if(err > dither_args->threshold * dither_args->threshold){
                dither_args->image->setPixel(x, y, newColor);
            }else{
                dither_args->image->setPixel(x, y, quantOldColor);
            }
        }
    }
    pthread_exit(NULL);
}

void threadDitherFloydSteinberg(sf::Image& image, std::function<RGB(const RGB&)> &quant, float threshold)
{
    sf::Vector2u imgSize = image.getSize();
    pthread_t threads[THREAD_NUM];
    struct dither_args dither_args[THREAD_NUM];
    int num_of_rows = (imgSize.y + THREAD_NUM - 1) / THREAD_NUM;

    for (int i = 0; i < THREAD_NUM; i++) {
        dither_args[i].thread_id = i;
        dither_args[i].num_of_rows = num_of_rows;
        dither_args[i].image = &image;
        dither_args[i].quant = quant;
        dither_args[i].threshold = threshold;
        if (pthread_create(&threads[i], NULL, thread_dither_floydsteinberg, (void *) &dither_args[i]) != 0) {
            printf("Thread creation failed\n");
            exit(-1);
        }
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Thread join failed\n");
            exit(-1);
        }
    }
}

void *thread_dither_floydsteinberg(void *args)
{
    struct dither_args *dither_args = (struct dither_args *) args;
    double N = dither_args->m.getHeight() * dither_args->m.getWidth();
    sf::Vector2u imgSize = dither_args->image->getSize();

    for(uint k = 0; k < dither_args->num_of_rows; k++) {
        uint y = dither_args->num_of_rows * dither_args->thread_id + k; // 從 thread id * num_of_row 開始畫
        for(uint x = 0; x < imgSize.x; x++){
            RGB oldColor = dither_args->image->getPixel(x,y);
            RGB newColor = dither_args->quant(oldColor);
            dither_args->image->setPixel(x,y,newColor); 
            float err = rgbSquaredDistance(oldColor, newColor);
            if (err > dither_args->threshold * dither_args->threshold) {
                float rErr = (float)oldColor.r - newColor.r;
                float gErr = (float)oldColor.g - newColor.g;
                float bErr = (float)oldColor.b - newColor.b;
                auto updatePixel = [&](uint xi, uint yi, float t){
                    if (xi >= imgSize.x || yi >= imgSize.y)
                        return; // unsigned so negative overflow
                    RGB p = dither_args->image->getPixel(xi, yi);
                    p.r = std::max(0.f, std::min(255.f, (float)p.r + (rErr * t)));
                    p.g = std::max(0.f, std::min(255.f, (float)p.g + (gErr * t)));
                    p.b = std::max(0.f, std::min(255.f, (float)p.b + (bErr * t)));
                    dither_args->image->setPixel(xi, yi, p);
                };
                updatePixel(x+1, y+1, 1.f/16);
                updatePixel(x-1, y+1, 3.f/16);
                updatePixel(x, y+1, 5.f/16);
                updatePixel(x+1, y, 7.f/16);
            }
        }
    }
    pthread_exit(NULL);
}