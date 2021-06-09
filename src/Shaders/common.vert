#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 1.0);
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}