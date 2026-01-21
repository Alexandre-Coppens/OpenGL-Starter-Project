#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec4 ourColor;

uniform vec3 offset;

void main()
{
    gl_Position = vec4(pos.x + offset.x, pos.y + offset.y, pos.z, 1.0);
    ourColor = vec4(pos, 1.0);
}