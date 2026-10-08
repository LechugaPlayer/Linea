#include "lin.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstdio>

constexpr uint32_t WIDTH = 600;
constexpr uint32_t HEIGHT = 800;

uint32_t* pixels = nullptr;

bool writePPM( const char* path){
    FILE* file = std::fopen(path, "wb");

    if (!file) {
        return false;
    }
    std::fprintf(file, "P6\n%u %u\n 255 \n",
                 WIDTH, HEIGHT);
    for (uint32_t y = 0; y < HEIGHT; y++) {
        for (uint32_t x = 0; x < WIDTH; x++) {
           uint32_t pixel = pixels[y * WIDTH + x];

           uint8_t r = (pixel >> 0 ) & 0xFF;
           uint8_t g = (pixel >> 8 ) & 0xFF;
           uint8_t b = (pixel >> 16) & 0xFF;

            std::fputc(r, file);
            std::fputc(g, file);
            std::fputc(b, file);
        }
    }
    std::fclose(file);

    return true;
}

int main(){
    pixels = (uint32_t*)malloc(WIDTH * HEIGHT * sizeof(uint32_t));
    lin::init(WIDTH, HEIGHT, pixels);

    lin::drawLine(0, 0, 300, 400, lin::White);
    lin::fillCircle(50, 50, 50, lin::Red);
    
    if (! writePPM("Test.ppm")) {
    printf("Failed creation failed\n");

    return 1;
    }
    
    return 0;
}
