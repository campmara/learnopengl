# MATERIALS

We can place color properties for each stage of the phong lighting model in a Material structure in the shader, which we specify as a uniform so that the program can create Materials as it likes and send them up to the shader.

Example (fragment shader):
```c++
#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;
```

