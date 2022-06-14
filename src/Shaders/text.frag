#version 330 core

in vec2 texCoords;
out vec4 colour;

uniform sampler2D text;
uniform vec3 passed_colour;

void
main()
{
  vec4 sampled = vec4( 1.0, 1.0, 1.0, texture( text, texCoords ).r );
  colour= vec4( vec3( passed_colour ), 1.0f ) * sampled;
}
