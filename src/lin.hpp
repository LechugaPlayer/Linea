#include <cstdint>

namespace lin {
    void init(int WIDTH, int HEIGHT, uint32_t* framebuffer);
    void clear(uint32_t color);
    void drawPixel(int x, int  y, uint32_t color);
    void drawLine(int srcX, int srcY, int desX, int desY, uint32_t color);
    void drawRect(int minX, int minY, int maxX, int maxY, uint32_t color);
    void fillRect(int minX, int minY, int maxX, int maxY, uint32_t border, uint32_t body);
    void drawTriangle(int x1, int y1,int x2, int y2, int x3, int y3, uint32_t color);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t border, uint32_t body);
    void drawCircle(int centerX, int centerY, int radius, uint32_t color);
    void fillCircle(int centerX, int centerY, int radius, uint32_t color);
    void drawEllipse(int major, int minor, int centerX, int centerY, uint32_t color);
    void fillEllipse(int major, int minor, int centerX, int centerY, uint32_t color);
    void shutdown();
    
}
