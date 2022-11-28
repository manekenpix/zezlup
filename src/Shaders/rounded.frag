#version 330 core
out vec4 FragColour;

in vec2 texCoord;
uniform sampler2D passed_texture;
uniform vec3 passed_colour;
uniform bool has_border;
uniform bool has_texture;
uniform float fadeAlpha;

vec4 blank = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 colour = vec4( vec3(passed_colour), fadeAlpha );
float radius = 35.0f;
float cornerAlpha = 1.0;

void
main()
{
  float border_width = 0.02f;
  float maxX = 1.0f - border_width;
  float minX = border_width;

  bool is_not_border = texCoord.x < maxX && texCoord.x > minX && texCoord.y < maxX &&
    texCoord.y > minX;

  vec4 sampled_texture = texture( passed_texture, texCoord );
  vec4 selected_texture_for_rendering = has_texture ? sampled_texture : blank;
  selected_texture_for_rendering.w = fadeAlpha;

  vec2 texSize = textureSize(passed_texture, 0);
  vec2 quadCoords = texCoord * texSize;
  vec2 cornerDistance = min(quadCoords, texSize - quadCoords);
  if (max(cornerDistance.x, cornerDistance.y) <= radius) {
    cornerAlpha = radius - distance(cornerDistance, vec2(radius, radius));
  }

  if (has_border)
    FragColour = is_not_border ? selected_texture_for_rendering : colour;
  else {
    FragColour = selected_texture_for_rendering * clamp(cornerAlpha, 0.0, 1.0);
  }
}
