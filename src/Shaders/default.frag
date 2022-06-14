#version 330 core
out vec4 FragColour;

in vec2 texCoord;
uniform sampler2D our_texture;
uniform vec3 passed_colour;
uniform bool has_border;
uniform bool has_texture;

vec4 blank = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 colour = vec4( vec3(passed_colour), 1.0f );

void
main()
{
  float border_width = 0.020f;
  float maxX = 1.0f - border_width;
  float minX = border_width;

  bool is_not_border = texCoord.x < maxX && texCoord.x > minX && texCoord.y < maxX &&
    texCoord.y > minX;

  vec4 sampled_texture = texture( our_texture, texCoord );
  vec4 selected_texture_for_rendering = has_texture ? sampled_texture : blank;

  if (has_border)
    FragColour = is_not_border ? selected_texture_for_rendering : colour;
  else
    FragColour = selected_texture_for_rendering;
}
