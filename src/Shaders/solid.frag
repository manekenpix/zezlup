#version 330 core
out vec4 FragColour;

in vec2 texCoord;
uniform vec3 passed_colour;
vec4 colour = vec4( vec3(passed_colour), 1.0f );

void
main()
{
  FragColour = colour;
}
