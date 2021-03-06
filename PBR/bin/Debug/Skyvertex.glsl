#version 120

attribute vec3 position;

uniform mat4 view;
uniform mat4 projection;

varying vec3 TexCoords;

void main()
{
	gl_Position = projection * view * vec4(vec3(position.x * 200, (position.y * 200) + 100, position.z * 200), 1.0f);
	TexCoords = position;
}
