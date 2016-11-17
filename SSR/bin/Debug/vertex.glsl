#version 120

attribute vec3 position;
attribute vec2 uv;
attribute vec3 normal;

varying vec2 texCoords;
varying vec3 Normal;

uniform mat4 MVP;

void main()
{	
	gl_Position = MVP * vec4(position, 1.0);
	Normal = normal;
	texCoords = uv;
}