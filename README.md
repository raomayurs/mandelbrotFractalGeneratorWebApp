# Mandelbrot Fractal Generator

This package contains code for a web app to generate Mandelbrot fractals. Learn more about the Mandelbrot set [here](https://en.wikipedia.org/wiki/Mandelbrot_set).

The fractals are generated as 700x700 images. WebAssembly (WASM) is utilized for performance. The C code, responsible for generating Mandelbrot image pixels within specified bounds, is compiled to WASM. The WASM module is then loaded into JS and used in HTML to generate and render images. All generation occurs client-side in the browser.

## Usage

1. Visit [Mandelbrot App](https://raomayurs.github.io/mandelbrotFractalGeneratorWebApp/).
2. Select the `init` button to load the first fractal with bounds `x1 = -1.5, x2 = 0.5, y1 = -1.0, y2 = 1.0`.
3. Use `+` and `-` to zoom in or out.
4. Utilize direction buttons to navigate within the fractal's bounds.

## Development

- `mandelbrot.c` contains the C code to generate Mandelbrot pixels. The method `getMandelbrotImagePixels` accepts the width, height, and bounds for the Mandelbrot fractal, returning the pixel array.
- `index.html` contains code to render the fractals using the WASM module.

## Build Steps

### Build WASM for index.html

```bash
emcc mandelbrot.c -o mandelbrot.html -s EXPORTED_RUNTIME_METHODS=ccall -s TOTAL_STACK=15mb
```

### Build WASM for local NodeJS

```bash
emcc node/mandelbrotNodeJS.c -o node/mandelbrotNodeJS.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=ccall -s TOTAL_STACK=15mb
```
Run `node node/test.js` to execute a local NodeJS example to generate a fractal using WASM.

## Deployment of Web App

The Mandelbrot Fractal Generator web app is seamlessly hosted on Github using [Github Pages](https://pages.github.com).

Alternatively, if you prefer hosting on Amazon S3, follow these steps:

1. Upload the following files to your S3 bucket: `mandelbrot.js`, `mandelbrot.wasm`, and `index.html`.
2. Ensure that the uploaded files have public access permissions.
3. Utilize the public S3 URL for `index.html` to access and use the web app.

This flexibility allows you to choose the hosting solution that best suits your preferences.

