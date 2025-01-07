#version 330 core
out vec4 FragColor;

in vec3 GouraudColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(GouraudColor * objectColor, 1.0);
}