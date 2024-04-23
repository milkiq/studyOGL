#version 330 core

in vec3 ourColor;
out vec4 fragColor;

void func()
{
    // This is a function
}

void main()
{
    func();
    fragColor = vec4(ourColor, 1.0f);
}
