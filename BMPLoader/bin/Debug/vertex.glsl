#version 120

attribute vec3 position;
attribute vec2 texcoords;

varying vec2 UV;

void main()
{
	gl_Position = vec4(position, 1);
	UV = texcoords;
}