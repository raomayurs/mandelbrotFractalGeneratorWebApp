#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <png.h>

struct Complex {
    double r;
    double i;
};

struct Complex getMandelbrotComplexNumber(struct Complex z, struct Complex c) {
    struct Complex result; 
    result.r = z.r * z.r - z.i * z.i + c.r;
    result.i = 2 * z.r * z.i + c.i;
     
    return result;
}

int getIterations(struct Complex z, struct Complex c, int maxIterations) {
    int i = 0;
    struct Complex zn = getMandelbrotComplexNumber(z, c);
    while ((fabs(zn.r) <= 2.0 && fabs(zn.i) <= 2.0) && i < maxIterations) {
        zn = getMandelbrotComplexNumber(zn, c);
        i = i + 1;
     }
     return i;
}

int* getPixelValue(int iter) {
    int colors[16][3] = { 66, 30, 15, 25, 7, 26, 9, 1, 47, 4, 4, 73, 0, 7, 100, 12, 44, 138, 24, 82, 177, 57, 125, 209, 134, 181, 229, 211, 236, 248, 241, 233, 191, 248, 201, 95, 255, 170, 0, 204, 128, 0, 153, 87, 0, 106, 52, 3 };
    int pixel[3];
	int index = iter % 16;
	pixel[0] = colors[index][0];
	pixel[1] = colors[index][1];
	pixel[2] = colors[index][2];
	return pixel;
}

int* getPixelValueForJulia(int iter) {
    int colors[16][3] = { 0, 0, 0, 25, 7, 26, 9, 1, 47, 4, 4, 73, 0, 7, 100, 12, 44, 138, 24, 82, 177, 57, 125, 209, 134, 181, 229, 211, 236, 248, 241, 233, 191, 248, 201, 95, 255, 170, 0, 204, 128, 0, 153, 87, 0, 106, 52, 3 };
    int pixel[3];
	int index = iter % 16;
	pixel[0] = colors[index][0];
	pixel[1] = colors[index][1];
	pixel[2] = colors[index][2];
	return pixel;
}

int writeImage(char* filename, int width, int height, int *buffer, char* title)
{
	int code = 0;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width * 3 ; x++) {
			row[x] = buffer[y * (width*3) + x];
 		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}

int writeMandelbrotFractal(int width, int height, int maxIterations, double x1, double x2, double y1, double y2, char* filename) {
	int numPixels = height * width * 3;
    int pixels[height * width * 3];
    
    int i, j;
    int count = 0;
    unsigned long start = time(NULL);
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
			
        }
    }
    unsigned long end = time(NULL);
    printf("It took %lf ms", end - start);
    int result = writeImage(filename, width, height, pixels, "This is my test image");

	return result;
}

int writeJuliaFractal(int width, int height, int maxIterations, double r, double im, double x1, double x2, double y1, double y2, char* filename) {
	int numPixels = height * width * 3;
    int pixels[height * width * 3];

    
    int i, j;
    int count = 0;
    unsigned long start = time(NULL);
	FILE* pfile;
	pfile = fopen("pixels", "w");
	int c1 = 0;
    for (i = 0; i < width; i++) {
        for (j = 0; j< height; j++) {
			double y = (double)(((y2 - y1) * (double)i) / (double)width) + y1;
            double x = (double)(((x2 - x1) * (double)j) / (double)height) + x1;
			
			struct Complex z = {.r = x, .i = y };
			struct Complex c = {.r = r, .i = im };

			int iter = getIterations(z, c, maxIterations);
			
            int* pixelValue = getPixelValueForJulia(iter);
			fprintf(pfile, "%d %f %f %d\n",c1++, x, y, iter);
			// printf("%d %d %d %d\n",c1++, pixelValue[0], pixelValue[1], pixelValue[2]);
            pixels[count++] = pixelValue[0];
            pixels[count++] = pixelValue[1];
            pixels[count++] = pixelValue[2];
		}
	}
	unsigned long end = time(NULL);
    printf("It took %lf ms", end - start);
    int result = writeImage(filename, width, height, pixels, "This is my test image");

	return result;
}

int main(int argc, char** argv) {
    // int height = atoi(argv[1]);
    // int width = atoi(argv[2]);
    // int maxIterations = 50;

    // double x1 = atof(argv[3]);
    // double x2 = atof(argv[4]);

    // double y1 = atof(argv[5]);
    // double y2 = atof(argv[6]);

	// char* filename = argv[7];

	// return writeMandelbrotFractal(width, height, maxIterations, x1, x2, y1, y2, filename);

	int height = atoi(argv[1]);
    int width = atoi(argv[2]);
    int maxIterations = 50;

	double r = atof(argv[3]);
    double i = atof(argv[4]);

	double x1 = atof(argv[5]);
    double x2 = atof(argv[6]);

    double y1 = atof(argv[7]);
    double y2 = atof(argv[8]);

	char* filename = argv[9];

	return writeJuliaFractal(width, height, maxIterations, r, i, x1, x2, y1, y2, filename);
}