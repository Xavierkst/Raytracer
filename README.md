# Raytracer

A basic ray tracer written in C++. Work in progress (when time permits). All graphics knowledge obtained from various online sources such as [scratchapixel](https://www.scratchapixel.com) and [Raytracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

### Features included: 

* Blinn-Phong shading
* Reflections, recursive refraction 
* Fresnel effect
* Anti-aliasing (with jittered rays) 
* Soft shadows (Monte Carlo lighting) with Area lights
* Ray-Sphere/(Axis aligned)Box/Rectangle/Plane intersection routines

### In-progress: 

* Acceleration structure: Uniform grid 

### Future implementations:  

* Acceleration Structures (bounding volume hierarchy)
* Triangle Meshes 
* Spherical Lights 
* Glossy reflections (metal surfaces)
* Texture maps

### Cornell Box: Reflection and refraction

![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/Cornell_Box_objects.jpg)
![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/testFile_AA_8_rectangle.jpg)

### Axis Aligned Bounding Boxes (AABB) transmissive and diffuse surfaces

![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/4cubes.jpg)

### Show casing reflections, refractions, fresnel effect

![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/transparency_pic.jpg)

### Mirror/Reflective spheres ![Ray traced with Anti-aliasing depth 12](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/sphereArc.jpg) 

![Ray traced with Anti-aliasing depth 5](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/testFile_2_tinted_again.jpg)

### Extras from a raytracer I wrote using the Nvidia OptiX (general purpose ray tracing API) 

![Red Dragon](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/red_dragon.png)
 
Green dragon | Cornell box
-------------| ------------
![Green Dragon Monte Carlo](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/dragon.png) | ![Cornell Box](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/cornell.png)
-------------| ------------
![Room](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/scene6.png) | ![Balls on table - Specular](https://github.com/Xavierkst/Raytracer_build/blob/master/rendered_images/scene4-specular.png)

