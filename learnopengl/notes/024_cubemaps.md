# Cubemaps

A cubemap is a texture that contains 6 individual 2D textures that form a cube. Vector directions are mapped in OpenGL to texel coordinates on the cube. The magnitude of these directions don't matter.

```c++
// Creation
unsigned int textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
```

With six faces, we have to make six calls to `glTexImage2D`, and the `target` parameter has to match the specific face of the cubemap. This is done in a loop usually.

These are the texture targets (an enum):
* GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
* GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
* GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
* GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
* GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
* GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front

```c++
// Here's an example of looping through the faces to call glTexImage2D
int width, height, nrChannels;
unsigned char *data;  
for(unsigned int i = 0; i < textures_faces.size(); i++)
{
    data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    );
}
```

A cubemap can also have its wrapping and filtering methods specified:
```c++
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
```

Cubemaps are useful for a lot of things!

## Skybox

The pixel shader for a skybox needs to use *normalized device coordinates*:

```c++
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
```

And the fragment shader utilizes a `samplerCube` type:

```c++
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
```

## Environment Mapping

Reflections using the skybox cubemap can be achieved using OpenGL's `reflect` function.

The vertex shader for an object with reflectance looks like this:
```c++
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```

And the fragment shader is as follows:
```c++
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
```