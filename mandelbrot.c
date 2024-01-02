#include <stdio.h>
#include <emscripten/emscripten.h>
#include <math.h>



#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE struct Complex {
    double r;
    double i;
};

EXTERN EMSCRIPTEN_KEEPALIVE struct Complex getMandelbrotComplexNumber(struct Complex z, struct Complex c) {
    struct Complex result; 
    result.r = z.r * z.r - z.i * z.i + c.r;
    result.i = 2 * z.r * z.i + c.i;
     
    return result;
}

EXTERN EMSCRIPTEN_KEEPALIVE int getIterations(struct Complex z, struct Complex c, int maxIterations) {
    int i = 0;
    // ** EXTREMELY WIERD BEHAVIOUR WARNING**
    // DO NOT REMOVE THIS RESULT ARRAY
    // Removing it causes this method to get computed incorrectly
    double result[maxIterations * 3];
    int count = 0;
    struct Complex zn = getMandelbrotComplexNumber(zn, c);
    while ((fabs(zn.r) <= 2.0 && fabs(zn.i) <= 2.0) && i < maxIterations) {
        zn = getMandelbrotComplexNumber(zn, c);
        i = i + 1;
        result[count++] = zn.r;
        result[count++] = zn.i;
        result[count++] = (double)i;
    }
    return i;
}

EXTERN EMSCRIPTEN_KEEPALIVE int* getPixelValue(int iter) {
    int colors[16][3] = { 66, 30, 15, 25, 7, 26, 9, 1, 47, 4, 4, 73, 0, 7, 100, 12, 44, 138, 24, 82, 177, 57, 125, 209, 134, 181, 229, 211, 236, 248, 241, 233, 191, 248, 201, 95, 255, 170, 0, 204, 128, 0, 153, 87, 0, 106, 52, 3 };
    int pixel[3];
	int index = iter % 16;
	pixel[0] = colors[index][0];
	pixel[1] = colors[index][1];
	pixel[2] = colors[index][2];
	return pixel;
}

EXTERN EMSCRIPTEN_KEEPALIVE uintptr_t getIntArray()
{
    int values[3];
    values[0] = 76;
    values[1] = 2;
    values[2] = 3;
    return (uintptr_t)values;
}

EXTERN EMSCRIPTEN_KEEPALIVE uintptr_t getMandelbrotImagePixels(int width, int height, double x1, double x2,double y1, double y2) {
    int maxIterations = 50;
    double pixels[height * width * 4];
    
    int i, j;
    int count = 0;
    for (i = 0; i < width; i++) {
        for (j = 0; j< height; j++) {
            double y = (double)(((y2 - y1) * (double)i) / (double)width) + y1;
            double x = (double)(((x2 - x1) * (double)j) / (double)height) + x1;
            struct Complex z = {.r = 0, .i = 0 };
            struct Complex c = {.r = x, .i = y };
            int iter = getIterations(z, c, maxIterations);
            int* pixelValue = getPixelValue(iter);
            pixels[count++] = pixelValue[0];
            pixels[count++] = pixelValue[1];
            pixels[count++] = pixelValue[2];
            pixels[count++] = 255;
        }
    }
    return (uintptr_t)pixels;
}

int main(int argc, char** argv) {
    return 0;
}