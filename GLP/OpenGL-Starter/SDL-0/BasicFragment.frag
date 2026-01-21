#version 330 core
in vec2 fragCoord;
out vec4 FragColor;

void main()
{
    FragColor = vec4(fragCoord,1.0f, 1.0f);
}