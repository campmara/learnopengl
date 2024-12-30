# HELLO TRIANGLE

OpenGL's work is about transforming 3D coordinates into 2D pixels. This is managed by the *graphics pipeline*.

Graphics pipeline has two large parts:
1. transform 3d coords into 2d coords
2. transforms 2d coords into colored pixels

It can be divided into several steps, with each step using the previous step as input. These steps are highly specialized and can be run concurrently.

Processing cores on your GPU run small programs for each step of the pipeline. These are called *shaders*.

OpenGL shaders are written in GLSL.

Here are the discrete steps of the graphics pipeline:
1. Vertex Data[] goes into the vertex shader
2. result into geometry shader
3. result into shape assembly
4. result into rasterization
5. result into fragment shader
6. result into tests and blending

Vertex Data:
* usually represented by 3d coordinates and a color
* might have other information stored within

Vertex Shader:
* takes a single vertex as input
* transforms 3d coords into different 3d coords
* basic processing on vertex attributes

Geometry Shader:
* takes collection of vertices as input
* has the ability to generate other shapes by emitting new vertices to form new primitives.

Primitive Assembly:
* takes as input all vertices that form one or more primitives and assembles the points in the shape given

Rasterization:
* maps the resulting primitives to the corresponding pixels on screen
* results in fragments for the fragment shader to use
* clipping performed here

Fragment Shader:
* calculates the final color of a pixel
* all advanced effects occur here
* contains data about the 3d scene it can use to calculate the final color (lights, shadows, light color, etc.)

Alpha Test and Blending:
* Checks the depth and stencil value of each fragment and uses those to check if it is in front of or behind other objects
* Tells the program what fragments are occluded and can be discarded.
* Checks alpha (transparency) values and blends fragments accordingly

We are *required* to define at least a vertex and fragment shader of our own in OpenGL. No defaults here.

glBufferData function:
* GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
* GL_STATIC_DRAW: the data is set only once and used many times.
* GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

Vertex Stride:
* the space between consecutive vertex attributes

## THE PROCESS
```c++
// ..:: Initialization code :: ..
// 1. bind Vertex Array Object
glBindVertexArray(VAO);
// 2. copy our vertices array in a vertex buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. copy our index array in a element buffer for OpenGL to use
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 4. then set the vertex attributes pointers
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  

[...]
  
// ..:: Drawing code (in render loop) :: ..
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
```

VBO: Vertex Buffer Object
* Manages the memory for vertex data
* we use this to try to send as much data up to the GPU at once as possible

VAO: Vertex Array Object
* stores vertex attribute calls
* makes drawing objects multiple times easy, just bind the corresponding VAO

EBO: Element Buffer Object
* Binds indices to the VBO

## Indices
These specify the index order of each triangle you would like to draw in the vertex array.