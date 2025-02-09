# Lighting Section Review

* Color vector: a vector portraying most of the real world colors via a combination of red, green and blue components (abbreviated to RGB). The color of an object is the reflected color components that an object did not absorb.
* Phong lighting model: a model for approximating real-world lighting by computing an ambient, diffuse and specular component.
* Ambient lighting: approximation of global illumination by giving each object a small brightness so that objects aren't completely dark if not directly lit.
* Diffuse shading: lighting that gets stronger the more a vertex/fragment is aligned to a light source. Makes use of normal vectors to calculate the angles.
* Normal vector: a unit vector that is perpendicular to a surface.
* Normal matrix: a 3x3 matrix that is the model (or model-view) matrix without translation. It is also modified in such a way (inverse-transpose) that it keeps normal vectors facing in the correct direction when applying non-uniform scaling. Otherwise normal vectors get distorted when using non-uniform scaling.
* Specular lighting: sets a specular highlight the closer the viewer is looking at the reflection of a light source on a surface. Based on the viewer's direction, the light's direction and a shininess value that sets the amount of scattering of the highlight.
* Phong shading: the Phong lighting model applied in the fragment shader.
* Gouraud shading: the Phong lighting model applied in the vertex shader. Produces noticeable artifacts when using a small number of vertices. Gains efficiency for loss of visual quality.
* GLSL struct: a C-like struct that acts as a container for shader variables. Mostly used for organizing input, output, and uniforms.
Material: the ambient, diffuse and specular color an object reflects. These set the colors an object has.
* Light (properties): the ambient, diffuse and specular intensity of a light. These can take any color value and define at what color/intensity a light source shines for each specific Phong component.
* Diffuse map: a texture image that sets the diffuse color per fragment.
* Specular map: a texture map that sets the specular intensity/color per fragment. Allows for specular highlights only on certain areas of an object.
* Directional light: a light source with only a direction. It is modeled to be at an infinite distance which has the effect that all its light rays seem parallel and its direction vector thus stays the same over the entire scene.
* Point light: a light source with a location in a scene with light that fades out over distance.
* Attenuation: the process of light reducing its intensity over distance, used in point lights and spotlights.
* Spotlight: a light source that is defined by a cone in one specific direction.
* Flashlight: a spotlight positioned from the viewer's perspective.
* GLSL uniform array: an array of uniform values. Work just like a C-array, except that they can't be dynamically allocated