version 110 

uniform sampler2D texture;

varying vec2 texCoords;

#include <file.glsl>

void main()
{
	gl_FragColor = texture2D(texture, texCoords);
} 
