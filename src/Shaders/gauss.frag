#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D ourTexture;

void main() {
	float Pi = 6.28318530718; // Pi*2
  
  // Gaussian settings
  float Directions = 16.0; // Blur Directions (Default 16.0 - More is better but slower)
  float Quality = 10.0; // Blur Quality (Default 4.0 - More is better but slower)
  float Size = 15.0; // Blur Size (Radius)
  
  vec2 Radius = Size / textureSize( ourTexture, 0 ).xy;
  
  // Pixel colour
  vec4 Color = texture( ourTexture, texCoord );
  
  // Blur calculations
  for( float d = 0.0; d < Pi; d += Pi / Directions )
  {
		for( float i = 1.0 / Quality; i <= 1.0; i += 1.0 / Quality )
    {
			Color += texture( ourTexture, texCoord + vec2( cos(d), sin(d)) * Radius * i );
    }
  }
  
  // Output to screen
  Color /= Quality * Directions - 15.0;
  FragColor =  Color;
}
