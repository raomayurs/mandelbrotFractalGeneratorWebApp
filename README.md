# Fractalx Generator

This package contains code for a web app to generate Mandelbrot and Julia fractals. Learn more about the Mandelbrot set [here](https://en.wikipedia.org/wiki/Mandelbrot_set). Learn more about Julia set [here](https://en.wikipedia.org/wiki/Julia_set)

The fractals are generated as 700x700 images. WebAssembly (WASM) is utilized for performance. The C code, responsible for generating Mandelbrot image pixels within specified bounds, is compiled to WASM. The WASM module is then loaded into JS and used in HTML to generate and render images. All generation occurs client-side on the browser in real time.

## Usage

### Mandelbrot Fractals

1. Visit [Mandelbrot App](https://raomayurs.github.io/mandelbrotFractalGeneratorWebApp/mandelbrot.html).
2. Select the `init` button to load the first fractal with bounds `x1 = -1.5, x2 = 0.5, y1 = -1.0, y2 = 1.0`.
3. Use `+` and `-` to zoom in or out.
4. Utilize direction buttons to navigate within the fractal's bounds.

### Julia Fractals

1. Visit [Julia App](https://raomayurs.github.io/mandelbrotFractalGeneratorWebApp/julia.html).
2. Select the `init` button to load the first fractal with bounds `x1 = -2, x2 = 2, y1 = -2, y2 = 2` and first imaginary `c = -0.4 + 0.65i`
3. Use zoom controls to zoom in or out.
4. Utilize direction buttons to navigate within the fractal's bounds.
5. Use the controls to update the value of the complex number c. This determines the shape of the fractal

## Development

### Prerequisites
- Emsdk - Emsdk contains tools like `emrun` and `emcc`. Please visit [here](https://emscripten.org/docs/getting_started/downloads.html)
- Libpng - If images need to be generated locally then please install libpng. 

### Info

- `fractal.c` contains the C functions to generate Mandelbrot and Julia fractals. 
- `mandelbrot.html` contains code to render the Mandelbrot fractals using WASM
- `julia.html` contains code to render the Julia fractals using WASM
- Run `emrun mandelbrot.html` to host the web app on a local server
    - Visit `http://localhost:6931/index.html` once the server starts to use the web app
    - Refresh on making changes to `index.html` to see changes
    - Same steps for `julia.html`
- `fractalsGCC.c` is a C program that is a very similar file to fractal.c. Just that this one can be run using gcc
    - `gcc -lm -lpng fractalsGCC.c -o fractal && ./fractal 10 10 -0.4 0.65 -2.0 2.0 -2.0 2.0 test.png`

## Build Steps

### Build WASM for index.html

```bash
emcc fractal.c -o fractal.html -s EXPORTED_RUNTIME_METHODS=ccall -s TOTAL_STACK=15mb
```

### Build WASM for local NodeJS

```bash
emcc fractal.c -o fractalNodeJS.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=ccall -s TOTAL_STACK=15mb
```
Run `node test.js` to execute a local NodeJS example to generate a fractal using WASM.

## Deployment of Web App

The web app is seamlessly hosted on Github using [Github Pages](https://pages.github.com).
