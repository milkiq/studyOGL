#version 450

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 FragPos;

layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform sampler2D bgTexture;
layout (binding = 1) uniform sampler2D faceTexture;

struct Light {
    vec3 position;
    vec3 color;
};

// in high level glsl, every uniform non-block variable need layout(location=L)
// but set value still like this: glUniform1f(glGetUniformLocation(shader.ID, "mix_value"), 0.2f);
layout (location = 0) uniform float mix_value;

layout (std140, binding = 0) uniform Matrices {
    mat4 model_matrix;
    mat4 view_matrix;
    mat4 projection_matrix;
    mat3 normal_matrix;
};

layout (std430, binding = 1) buffer LightData {
    Light light;
};

void main()
{
    vec4 bg = texture(bgTexture, TexCoord);
    vec4 face = texture(faceTexture, TexCoord);

    vec4 base_color;
    if (face.a > 0.1f)
    {
        base_color = mix(bg, face, mix_value);
    } else {
        base_color = bg;
    }

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light.color;

    vec3 light_view_pos = vec3(view_matrix * vec4(light.position, 1.0));
    vec3 light_dir = normalize(light_view_pos - FragPos);
    float diff = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse = diff * light.color;

    float specular_strength = 1;
    vec3 view_pos = vec3(0.0, 0.0, 0.0);
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
    vec3 specular = specular_strength * spec * light.color;

    vec3 result = (ambient + diffuse + specular) * base_color.rgb;

    fragColor = vec4(result, 1.0);
}
