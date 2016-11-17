#version 120

attribute vec3 position;
attribute vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec3 COL;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	gl_PointSize = 4.0;
    	
	COL = color;
}
