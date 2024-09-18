#version 450

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 FragPos;

layout (location = 0) out vec4 fragColor;

// in high level glsl, every uniform non-block variable need layout(location=L)
// but set value still like this: glUniform1f(glGetUniformLocation(shader.ID, "mix_value"), 0.2f);
layout (location = 0) uniform vec3 light_color;

void main()
{
    fragColor = vec4(light_color, 1.0f);
}
