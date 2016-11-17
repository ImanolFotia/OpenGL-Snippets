version 110 

uniform sampler2D texture;

varying vec2 texCoords;


uniform float time;

void my_func()
{
	blablabla;
}



void main()
{
	gl_FragColor = texture2D(texture, texCoords);
} 

