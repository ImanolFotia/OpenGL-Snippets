#version 110

attribute vec2 position;

uniform float time;
uniform vec2 mousepos;

varying vec2 line;
void main()
{	
	mousepos;
	gl_Position = vec4(mousepos.x, 1.0, 0.0, 1.0f);
	
	line.x = ((position.x+ 1.0) / 2.0);
	line.y = position.y; 
}
