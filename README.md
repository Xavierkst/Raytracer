## Raytracer README
A basic ray tracer written in C++ that I will continue to develop. I am curious about the process of achieving realism in the animations/games seen today. I wanted to learn how I could build something more than just a bare-bones ray tracer as seen in Peter Shirley's [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).
There is no multi-threading in this and so the rendering time is very long without the help of an acceleration structure (Which I'm building at the moment).

### Features included: 
* Blinn-Phong shading
* Reflections, recursive refraction 
* Fresnel effect
* Anti-aliasing (with jittered rays) 
* Soft shadows (Monte Carlo lighting) with Area lights
* Ray-Sphere/(Axis aligned)Box/Rectangle/Plane intersection routines
### In-progress: 
* Acceleration structure: Uniform grid 
### Cornell Box: Reflection and refraction
![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/Cornell_Box_objects.jpg)
![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/testFile_AA_8_rectangle.jpg)
### 4 Boxes (AABB) transmissive and diffuse surfaces
![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/4cubes.jpg)
### Show casing reflections, refractions, fresnel effect
![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/transparency_pic.jpg)
### Mirror/Reflective spheres
![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/sphereArc.jpg)
### Future implementations:  
* Acceleration Structures (bounding volume hierarchy)
* Triangle Meshes 
* Spherical Lights 
* Glossy reflections (metal surfaces)
* Texture maps
### Extras
![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/testFile_2_tinted_again.jpg)
