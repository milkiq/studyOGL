#version 450

layout (location = 0) in vec3 ourColor;
layout (location = 1) in vec2 TexCoord;

layout (location = 0) out vec4 fragColor;

layout (location = 0) uniform vec3 light_color;

void main()
{
    fragColor = vec4(light_color, 1.0f);
}
