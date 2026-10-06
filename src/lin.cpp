#include "lin.hpp"
#include <stdlib.h>
#include <cstdint>
#include <Windows.h>

int width;
int height;
uint32_t* buffer = nullptr;

void lin::init(int WIDTH, int HEIGHT, uint32_t* framebuffer){
    width = WIDTH;
    height = HEIGHT;  
    buffer = framebuffer;
}

void lin::clear(uint32_t color){
   for (int y = 0; y < height; y++) {
     for (int x = 0; x < width; x++) {
       buffer[y * width + x] = color;
     }
   } 
}

void lin::drawPixel(int x, int y, uint32_t color){
    buffer[y * width + x] = color;
}

void lin::drawLine(int srcX, int srcY, int desX, int desY, uint32_t color){
//bresenham's algorithm
  int dx = abs(desX - srcX);
  int dy = abs(desY - srcY);
  int sx = (srcX < desX) ? 1 : -1;
  int sy = (srcY < desY) ? 1 : -1;
  int err = dx - dy;

  while (true) {
    int offset = srcY * width + srcX;
    
    buffer[offset] = color;

    if (srcX == desX && srcY == desY) break;
    int e2 = 2* err;
    if (e2 > -dy) {
      err -= dy;
      srcX += sx;
    }
    if (e2 < dx) {
      err += dx;
      srcY += sy;
    }
  }
}

void lin::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color){
    lin::drawLine(x1, y1, x2, y2,color);
    lin::drawLine(x2, y2, x3, y3,color);
    lin::drawLine(x1, y1, x3, y3,color);
}

void lin::drawRect(float minX, float minY, float maxX, float maxY, uint32_t color){
  lin::drawLine(minX, minY, minX , maxY, color);
  lin::drawLine(minX, minY, maxX , minY, color);
  lin::drawLine(maxX, maxY, minX , maxY, color);
  lin::drawLine(maxX, minY, maxX, maxY,  color);
  
}

void plotCirclePoints(int cx, int cy, int x, int y, uint32_t color) {
  lin::drawPixel(cx + x, cy + y, color);
  lin::drawPixel(cx - x, cy + y, color);
  lin::drawPixel(cx + x, cy - y, color);
  lin::drawPixel(cx - x, cy - y, color);
  lin::drawPixel(cx + y, cy + x, color);
  lin::drawPixel(cx - y, cy + x, color);
  lin::drawPixel(cx + y, cy - x, color);
  lin::drawPixel(cx - y, cy - x, color);
}


void lin::drawCircle(int centerX, int centerY, int radius, uint32_t color){
  
  int x = 0;
  int y = radius;
  int p = 1 - radius;

  while (x < y) {
    x++;
    if (p < 0) {
      p += 2 * x + 1;
    } else {
      y--;
      p += 2 * (x - y) + 1;
  }

  plotCirclePoints(centerX, centerY, x, y, color);
  }
  

}
void plotEllipsePoints(int cx, int cy, int x, int y, uint32_t color) {
  lin::drawPixel(cx + x, cy + y, color);
  lin::drawPixel(cx - x, cy + y, color);
  lin::drawPixel(cx + x, cy - y, color);
  lin::drawPixel(cx - x, cy - y, color);
}

  void lin::drawEllipse(int major, int minor, int centerX, int centerY, uint32_t color){
    
  int dx, dy, d1, d2, x , y;    
  x = 0;
  y = minor;
  
  d1 = (minor * minor) - ( major * major) + (0.25 * major * major);
  dx = 2 * minor * minor * x;
  dy = 2 * major * major * y;

  while (dx < dy) {
  plotEllipsePoints(centerX, centerY, x, y, color);
    if (d1 < 0) {
      
      x++;
      dx = dx + (2 * minor * minor);
      d1 = d1 + dx + (minor * minor);
    }else{
      x++;
      y--;
      dx = dx + (2 * minor * minor);
      dy = dy - (2 * major *major);
      d1 = d1 + dx - dy + (minor * minor);
    }
  }


   d2 = ((minor * minor) * ((x + 0.5) * (x + 0.5)))
         + ((major * major) * ((y - 1) * (y - 1)))
         - (major * major * minor * minor);
   while (y >= 0) {
     plotEllipsePoints(centerX, centerY, x, y, color);

     if (d2 > 0) {
       y--;
       dy =dy - (2 * major * major);
       d2 = d2 + (major * major) - dy;
     }else{
       y--;
       x++;
       dx = dx + (2 * minor * minor);
       dy = dy - (2 * major * major);
       d2 = d2 +dx -dy + (major * major);
     }
   }
    }
