const factory = require('./mandelbrotNodeJS.js');
const fs = require('fs');

factory().then((instance) => {
  const startTime = Date.now();
  const width = 50;
  const height = 50;
  const numEle = width * height * 3;
  const result = instance.ccall('getMandelbrotImagePixels','number', ['number', 'number', 'number', 'number', 'number', 'number'], [width, height, 0.3, 0.5, -0.3, -0.5]);
  const arrayData = []
  console.log(result)
  for (let v=0; v < numEle; v++) {
      arrayData.push( instance.HEAPF64[result/Float64Array.BYTES_PER_ELEMENT+v] )
  }

  let pixels = arrayData.reduce((rows, key, index) => (index % 3 == 0 ? rows.push([key]) : rows[rows.length-1].push(key)) && rows, []);
  // let image = pixels.reduce((rows, key, index) => (index % width == 0 ? rows.push([key]) : rows[rows.length-1].push(key)) && rows, []);
  for (let pixel of pixels) {
    // console.log(pixel);
    fs.appendFileSync( "pixelsFromWasm", `(${pixel.slice(0, 3).join(" ")}) \n`);
  }
  console.log(`Took ${(Date.now() - startTime)/1000}s to complete`);
});