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

Then, specifying material properties on the CPU looks like this:
```c++
lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
lightingShader.setFloat("material.shininess", 32.0f);
```