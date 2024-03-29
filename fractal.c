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
    struct Complex zn = getMandelbrotComplexNumber(z, c);
    while ((fabs(zn.r) <= 2.0 && fabs(zn.i) <= 2.0) && i < maxIterations) {
        zn = getMandelbrotComplexNumber(zn, c);
        i = i + 1;
        result[count++] = zn.r;
        result[count++] = zn.i;
        result[count++] = (double)i;
    }
    return i;
}

EXTERN EMSCRIPTEN_KEEPALIVE int k(float n, float h) {
	return (int)(n + h / 60) % 6;
}

EXTERN EMSCRIPTEN_KEEPALIVE double max(double a, double b) {
	if (a > b) return a;
	return b;
}

EXTERN EMSCRIPTEN_KEEPALIVE double min(double a, double b) {
	if (a < b) return a;
	return b;
}

EXTERN EMSCRIPTEN_KEEPALIVE double f(float n, float b, float s, float h) {
	return b * (1 - s * max(0, min(min(k(n, h), 4 - k(n, h)), 1)));
}

EXTERN EMSCRIPTEN_KEEPALIVE int* HSBtoRGB(float h, float s, float b) {
	int rgb[3];
	rgb[0] = 255 * f(5, b, s, h);
	rgb[1] = 255 * f(3, b, s, h);
	rgb[2] = 255 * f(1, b, s, h);
	return rgb;
}


EXTERN EMSCRIPTEN_KEEPALIVE double complexAbs(struct Complex z) {
	return sqrt(z.r * z.r + z.i * z.i);
}

EXTERN EMSCRIPTEN_KEEPALIVE double getSmoothIteration(struct Complex z, struct Complex c, int maxIterations) {
	int i = 0;
    struct Complex zn = getMandelbrotComplexNumber(z, c);
	double s = exp(-1 * complexAbs(zn));
    while ((fabs(zn.r) <= 2.0 && fabs(zn.i) <= 2.0) && i < maxIterations) {
        zn = getMandelbrotComplexNumber(zn, c);
        i = i + 1;
		s += exp(-1 * complexAbs(zn));
     }
     return s;
}

EXTERN EMSCRIPTEN_KEEPALIVE int* getSmoothPixelValue(double s1) {
    float h = 0.6+0.3*(sin(s1 * 0.1 * 1.1 * 0.63));
	float s = h * h;
	float v = h * s;
	return HSBtoRGB(h, s, v);
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

EXTERN EMSCRIPTEN_KEEPALIVE int* getPixelValueForJulia(int iter) {
    int colors[16][3] = { 0, 0, 0, 25, 7, 26, 9, 1, 47, 4, 4, 73, 0, 7, 100, 12, 44, 138, 24, 82, 177, 57, 125, 209, 134, 181, 229, 211, 236, 248, 241, 233, 191, 248, 201, 95, 255, 170, 0, 204, 128, 0, 153, 87, 0, 106, 52, 3 };
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

EXTERN EMSCRIPTEN_KEEPALIVE uintptr_t getJuliaFractalPixels(int width, int height, double r, double im, double x1, double x2, double y1, double y2) {
	int maxIterations = 50;
    int pixels[height * width * 4];

    int i, j;
    int count = 0;
    int c1 = 0;
    for (i = 0; i < width; i++) {
        for (j = 0; j< height; j++) {
			double y = (double)(((y2 - y1) * (double)i) / (double)width) + y1;
            double x = (double)(((x2 - x1) * (double)j) / (double)height) + x1;
			
			struct Complex z = {.r = x, .i = y };
			struct Complex c = {.r = r, .i = im };

			// int iter = getIterations(z, c, maxIterations);
			
            // int* pixelValue = getPixelValueForJulia(iter);
            double iter = getSmoothIteration(z, c, maxIterations);
			int* pixelValue = getSmoothPixelValue(iter);
            pixels[count++] = pixelValue[0];
            pixels[count++] = pixelValue[1];
            pixels[count++] = pixelValue[2];
            pixels[count++] = 255;
		}
	}
    return (uintptr_t)pixels;

}

EXTERN EMSCRIPTEN_KEEPALIVE uintptr_t getMandelbrotImagePixels(int width, int height, double x1, double x2,double y1, double y2) {
    int maxIterations = 50;
    int pixels[height * width * 4];
    
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