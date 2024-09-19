#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec3 FragPos;

layout (std140, binding = 0) uniform Matrices {
    mat4 model_matrix;
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 normal_matrix;
};

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
    Normal = mat3(normal_matrix) * aNormal;
    TexCoord = aTexCoord;
    FragPos = vec3(view_matrix * model_matrix * vec4(aPos, 1.0));
}
