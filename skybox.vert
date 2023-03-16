#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aTex;
    gl_Position = projection * view * vec4(aPos, 1.0);
}  