#version 450

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec2 TexCoord;

layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform sampler2D bgTexture;
layout (binding = 1) uniform sampler2D faceTexture;

// in high level glsl, every uniform non-block variable need layout(location=L)
// but set value still like this: glUniform1f(glGetUniformLocation(shader.ID, "mix_value"), 0.2f);
layout (location = 0) uniform float mix_value;

vec3 light_color = vec3(1.0, 1.0, 1.0);

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
    vec3 ambient = ambient_strength * light_color;

    vec3 light_dir = normalize(vec3(0.5, 0.5, 0.1));
    float diff = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient + diffuse) * base_color.rgb;

    fragColor = vec4(result, 1.0);
}
