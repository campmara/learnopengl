# Geometry Shader

Between the vertex and the fragment shader there's an optional shader stage: the geometry shader. It takes as input a set of vertices that form a single primitive (a point or a triangle). The geometry shader can then transform these vertices as it sees fit before passing off to the fragment shader stage.

The geometry shader is capable of converting the original primitives to a completely different set of primitives, possibly generating more vertices than given. I speculate this is how tesselation works.


Here's an example geometry shader:
```c++
#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
}  
```

First, we specify the type of primitive input we're receiving from the vertex shader (`layout (points) in;` section):
* `points`: drawing GL_POINTS primitives
* `lines`: drawing GL_LINES or GL_LINE_STRIP
* `lines_adjacency`: GL_LINES_ADJACENCY or GL_LINE_STRIP_ADJACENCY
* `triangles`: GL_TRIANGLES, GL_TRIANGLE_STRIP, or GL_TRIANGLE_FAN
* `triangles_adjacency`: GL_TRIANGLES_ADJACENCY or GL_TRIANGLE_STRIP_ADJACENCY

Then, we need to specify a primitive type that the shader will output:
* `points`
* `line_strip`
* `triangle_strip`

It also expects us to set a maximum number of vertices it outputs (if this number is exceeded, it won't draw the extra vertices).

The `gl_in` variable represents the output from the previous shader stage. It probably looks like this:
```c++
in gl_Vertex
{
    vec4  gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];
```

When we call `EmitVertex`, the vector currently set to gl_Position is added to the output primitive. When `EndPrimitive` is called, all the emitted vertices for the primitive are combined into the output render primitive.

Geometry shaders can be used to do cool stuff, like debugging the normals of a model, or even adding fur to an object.