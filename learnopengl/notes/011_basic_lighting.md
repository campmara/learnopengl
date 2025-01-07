# Basic Lighting

* Lighting in the real world is extremely complicated.
* In rendering, we work with simplified lighting models that are easier to understand, calculate, and look relatively similar.

First lighting model: Phong
* 3 components:
  * ambient
    * objects are almost never completely dark. this is a constant color that we build other parts of the model on top of
  * diffuse
    * simulates directional lighting. the more a part of an object faces a light source, the brighter it will be
  * specular
    * simulates the bright spots of light that appear on shiny objects. looks like the color of the light

## Ambient lighting

Ambient lighting is usually a strength value multiplied by the lightColor. It could also be a color value.

## Diffuse Lighting

Diffuse lighting is calculated with two factors:
* A normal vector from the surface (face) of an object
* The direction of the light

The dot product of these values is taken to generate the 'strength' of the diffuse lighting on that face. This is where the term 'NdotL' comes from!

## On Normals

Previously, we passed the normal vectors from the vertex shader to the fragment shader and performed calculations on them in there. However, all the calculations in the fragment shader are done in world space.

We can't multiply the model matrix by the normal so easily, though, because the normal is just a direction (has no w) and not a point in space. In order to perform that multiplication, we either need to multiply the normal by a 3x3 version of the model matrix or by the 4x4 matrix with the normal's w component set to 0.

Also, non-uniform scales of a model won't work properly yet, and the normals won't be perpendicular to their surfaces.

The 'trick' of fixing these issues is to use a different model matrix specifically made for normal vectors. This is called the *normal matrix*.

> normal matrix = transpose of the inverse of the upper left 3x3 of the model matrix

Doing the inverse of a matrix in the shader is costly, so in a real application you want to calculate it on the CPU and then send the resulting matrix to the shader via uniform.

## Specular Lighting

Similar to Diffuse in that it's based on the light direction and the normal vector of the surface, but it also factors in the view direction of the camera. Think of the object as a mirror. The highlight will be strongest when you see the light through that mirror.

LearnOpenGL.com is choosing to calculate this highlight in world space for easier understanding, but normally you'd do this in view space (since (0, 0, 0) would be the camera's position and you'd get the view pos for free). To do that, all you need to do is multiply all the relevant vectors (and the normal matrix) with the view matrix.