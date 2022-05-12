#version 330

out vec4 FragColor;
;

in vec2 texCoord;

void
main()
{
  float borderWidth = 0.025f;
  vec4 colour = vec4( 0.55f, 0.0f, 0.55f, 1.0f );
  
  float maxX = 1.0f - borderWidth;
  float minX = borderWidth;
  
  if ( texCoord.x < maxX && texCoord.x > minX && texCoord.y < maxX &&
      texCoord.y > minX ) {
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
    FragColor = colour;
  }
}
