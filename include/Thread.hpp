#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include <string>
#include <functional>
#include <cmath>
#include "Palette.hpp"
#include "Quantization.hpp"


#define THREAD_NUM 2
#define THREAD_ENABLED

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


struct dither_args {
    int thread_id;
    int num_of_rows;
    Matrix m;
    double sparsity;
    float threshold;
    sf::Image *image;
    std::function<RGB(const RGB&)> quant;
};

void *thread_min_max(void *arg);
void *thread_normalize(void *args);
void *thread_pixel(void *args);
void *thread_direct_quantize(void *args);
void *thread_dither_ordered(void *args);
void *thread_dither_floydsteinberg(void *args);


void threadDirectQuantize(sf::Image& image, std::function<RGB(const RGB&)> &quant);
void threadDitherOrdered(sf::Image& out, std::function<RGB(const RGB&)> &quantizer, const Matrix& m, double sparsity, float threshold = 0);
void threadDitherFloydSteinberg(sf::Image& image, std::function<RGB(const RGB&)> &quant, float threshold = 0);

#endif