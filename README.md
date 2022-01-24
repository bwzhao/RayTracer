# Path Tracer with multi sampling technics
## Overview


## How to run
### 1. Clone and build
```sh
git clone git@github.com:bwzhao/RayTracer.git
git submodule update --init --recursive
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
```
### 2. Running
Right now there are two examples:
1. Forward path tracing for a Cornell Box.
2. Bidirectional path tracing for a Cornell Box with reversed directions of light.

## Features:
* ### Lighting: 
  * [x] area light
  * [ ] point light
  * [ ] multi light source
* ### Integrator:
  * [x] path tracing
  * [x] bidirectional path tracing
  * [ ] metropolis light transport
* ### Primitives: 
  * [x] triangle
  * [ ] mesh triangle
  * [ ] circle
  * [x] rectangles
  * [x] sphere
* ### Texture type:
  * [x] pure color
  * [x] image
  * [x] noise
* ### Material type:
  * [x] Lambertian
  * [x] metal
  * [x] dielectric
  * [x] constant medium
* ### Obj loader
* ### BVH structure

## Appendix
* References:
    * Peter Shirley's famous trilogy of Ray tracing: https://raytracing.github.io
    * The PBR book: https://pbr-book.org/3ed-2018/contents
    * Advanced Global Illumination 2nd Edition

* Submodule needed: 
    * tinyobjloader: https://github.com/tinyobjloader/tinyobjloader.git