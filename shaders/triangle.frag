#version 450

layout (location = 0) in vec3 ourColor;
layout (location = 1) in vec2 TexCoord;

layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform sampler2D bgTexture;
layout (binding = 1) uniform sampler2D faceTexture;

// in high level glsl, every uniform non-block variable need layout(location=L)
// but set value still like this: glUniform1f(glGetUniformLocation(shader.ID, "mix_value"), 0.2f);
layout (location = 0) uniform float mix_value;

void main()
{
    fragColor = vec4(ourColor, 1.0f);
    // vec4 bg = texture(bgTexture, TexCoord);
    // vec4 face = texture(faceTexture, TexCoord);
    // if (face.a > 0.1f)
    // {
    //     fragColor = mix(bg, face, mix_value);
    // } else {
    //     fragColor = bg;
    // }
    // fragColor = texture(bgTexture, TexCoord) * vec4(ourColor, 1.0f);
}
