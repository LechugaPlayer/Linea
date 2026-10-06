#include <cstdint>

namespace lin {
    void init(int WIDTH, int HEIGHT, uint32_t* framebuffer);
    void clear(uint32_t color);
    void drawPixel(int x, int  y, uint32_t color);
    void drawLine(int srcX, int srcY, int desX, int desY, uint32_t color);
    void drawRect(float minX, float minY, float maxX, float maxY, uint32_t color);
    void drawTriangle(float x1, float y1,float x2, float y2, float x3, float y3, uint32_t color);
    void drawCircle(float centerX, float centerY, float radius, uint32_t color);
    void shutdown();
}
