# Advanced GLSL

## GLSL's Built-in Variables

Apart from `gl_FragCoord` and `gl_Position`, GLSL provides a few extra built-in variables prefixed with gl_ that we can use.

### Vertex Shader Variables

`gl_PointSize`:
Another render primitive we're able to choose from (GL_POINTS). In this case, every vertex is rendered as a point. It's possible to set the size of these points with gl_PointSize. This behaviour is disabled by default, so we must enable it with `glEnable(GL_PROGRAM_POINT_SIZE);`.

This could potentially be useful for stuff like particles!

`gl_VertexID`:
An input variable, which holds the current ID of the vertex we're drawing. When you do *indexed rendering* with `glDrawElements` this variable holds the index of the vertex we're drawing. Without indexed drawing (via `glDrawArrays`) this variable holds the number of the currently processed vertex since the start of the render call.

### Fragment Shader Variables

`gl_FragCoord`:
We've seen this before, because the z component of gl_FragCoord is the depth value of that particular fragment. We can also use the x and y components for some interesting effects.

The x and y components of gl_FragCoord are basically the screen-space coordinates of the fragment, originating from the bottom-left of the window.

By testing the x or y components and rendering differently based on which side of the screen the fragment is on, we can do side-by-side effects for graphics testing.

`gl_FrontFacing`:
Tells us if the current fragment is part of a front facing or back facing face. We could decide to output different colors for front vs back faces! If you have face culling enabled, however, you won't be able to really utilize this.

`gl_FragDepth`:
This lets us manually set the depth value of the fragment in the shader.

```c++
gl_FragDepth = 0.0;
```

Be careful doing this, though, as it prevents OpenGL from being able to do early depth testing, and will thus be slower.

From OpenGL 4.2 we can circumvent this somewhat by redeclaring gl_FragDepth at the top of the fragment shader with a depth condition:

```c++
layout (depth_<condition>) out float gl_FragDepth;
```

The condition can be any of the following:
* depth_any, default, early depth testing is disabled
* depth_greater, you can only make the depth value larger compared to gl_FragCoord.z
* depth_less, you can only make the depth value smaller compared to gl_FragCoord.z
* depth_unchanged, If you write to gl_FragDepth, you will write exactly the gl_FragCoord.z value

## Interface Blocks

Instead of declaring input and output data piece by piece in the vertex and fragment shaders, we can group them together in an interface block.

```c++
// In the vertex shader:
out VS_OUT
{
    vec2 TexCoords;
} vs_out;

// In the fragment shader:
in VS_OUT
{
    vec2 TexCoords;
} fs_in;
```

As long as both interface block names are equal, their corresponding input and output is matched together.

## Uniform Buffer Objects

Until now, we've had to continuously set uniform variables in each shader when most of the uniforms are exactly the same for each shader. For this, OpenGL offers us uniform buffer objects which allow us to declar *global* uniform values that remain the same across shader programs. Because a uniform buffer object is a buffer like any other, we create one via `glGenBuffers`, bind it to the `GL_UNIFORM_BUFFER` and store all the relevant data.

```c++
#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  
```

The `(std140)` sets the uniform block layout for the buffer object. OpenGL doesn't really specify the padding between variables in a block, and lets the hardware decide that for itself, but that makes it inconvenient for us since we want to deal with specific memory locations. The std140 layout makes the padding rules explicit for the block.

Each variable has a base alignment equal to the space it takes up (including padding) in std140. For each variable, we calculate its *aligned offset*, or the byte offset of a variable from the start of a block. The aligned byte offset MUST be equal to a multiple of the base alignment.

```c++
layout (std140) uniform ExampleBlock
{
                     // base alignment  // aligned offset
    float value;     // 4               // 0 
    vec3 vector;     // 16              // 16  (offset must be multiple of 16 so 4->16)
    mat4 matrix;     // 16              // 32  (column 0)
                     // 16              // 48  (column 1)
                     // 16              // 64  (column 2)
                     // 16              // 80  (column 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
}; 
```

### Using Uniform Buffers

```c++
unsigned int uboExampleBlock;
glGenBuffers(1, &uboExampleBlock);
glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

In order to set up a uniform buffer we also have to set a *binding point* for that buffer with `glUniformBlockBinding`:

```c++
unsigned int lights_index = glGetUniformBlockIndex(shaderA.ID, "Lights");   
glUniformBlockBinding(shaderA.ID, lights_index, 2);

// or, in OpenGL 4.2 on we can do this in the shader:
layout(std140, binding = 2) uniform Lights { ... };
// This will let us avoid having to call glGetUniformBlockIndex and glUniformBlockBinding
```

Then we bind the uniform buffer object to the same binding point:

```c++
glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock); 
// or
glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);
```

Now that everything is set up, we can start adding data to the uniform buffer:

```c++
glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
int b = true; // bools in GLSL are represented as 4 bytes, so we store it in an integer
glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b); 
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

UBOs have a lot of advantages over single uniforms:
* Setting a lot of uniforms at once is faster.
* If you want to change the same uniform over multiple shaders, it's easier.
* You can use a LOT more uniforms in shaders using them. OpenGL has a uniform limit (`GL_MAX_VERTEX_UNIFORM_COMPONENTS`). This will come in handy when you do skeletal animation.