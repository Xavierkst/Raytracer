## Raytracer README
A basic ray tracer I wrote in C++ 


### Features included: 
* Blinn-Phong shading
* Reflections, recursive refraction 
* Fresnel effect
* Anti-aliasing (with jittered rays) 
* Soft shadows (Monte Carlo lighting) with Area lights
* Ray-Sphere/Box/Rectangle/Plane intersection routines
### L & Center: dielectric spheres, Right: A reflective (mirror) sphere 
![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/testFile_AA_8_Box_render.jpg)
![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/testFile_AA_8_rectangle.jpg)
### 3 dielectric spheres, ea different refractive indices (fresnel seen in rightmost)
![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/testFile_1.jpg)
### L & R: mirror spheres, Center: dielectric
![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/testFile_AA_5_reflections.jpg)
### Upcoming features to implement:  
* Acceleration Structures (bounding volume hierarchy)
* Triangle Meshes 
* Spherical Lights 
* Glossy reflections (metal surfaces)
* Texture maps
### Extra Images (tinted/non-tinted dielectrics) ![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/testFile_2_tinted_again.jpg)
