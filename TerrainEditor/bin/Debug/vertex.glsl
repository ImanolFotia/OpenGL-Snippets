#version 120

attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 MVP; 
uniform float time;

varying float color;
varying vec3 FragPos;
void main()
{
	gl_Position = MVP * vec4(position, 1.0f); //vec4(position.x, sin(position.x+ time), position.z, 1.0f);
	FragPos = position;
	
	color = sin(position.x+ time);
}
