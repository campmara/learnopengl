#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float horizontalOffset;

out vec3 vertexPos;
out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.x + horizontalOffset, -aPos.y, aPos.z, 1.0);
    vertexPos = gl_Position.xyz;
    ourColor = aColor;
}

