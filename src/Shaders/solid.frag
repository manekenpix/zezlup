#version 330 core
out vec4 FragColour;

in vec2 texCoord;
uniform vec3 passed_colour;
vec4 colour = vec4( vec3(passed_colour), 1.0f );

uniform float width;
uniform float height;
float radius = 3.0f;
float cornerAlpha = 1.0;

void
main()
{
  vec2 texSize = vec2(width, height);
  vec2 quadCoords = texCoord * texSize;
  vec2 cornerDistance = min(quadCoords, texSize - quadCoords);
  if (max(cornerDistance.x, cornerDistance.y) <= radius) {
    cornerAlpha = radius - distance(cornerDistance, vec2(radius, radius));
  }

  FragColour = colour * clamp(cornerAlpha, 0.0, 1.0);
}
