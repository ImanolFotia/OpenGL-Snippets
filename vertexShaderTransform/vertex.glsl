#version 110

attribute vec3 position;
attribute vec2 texcoords;

varying vec2 UV;
varying vec3 FragPos;
uniform mat4 MVP;
uniform float time;
uniform float multiplier;
uniform mat4 model;

uniform sampler2D sampler;

void main()
{	
	float desp = sin(position.x * 20.0 + time) * multiplier;
	
	gl_Position = MVP * vec4(position.x, texture2D(sampler, texcoords).r * 3.0f, position.z, 1.0f);
	FragPos = vec3(model * vec4(position.x, texture2D(sampler, texcoords).r * 3.0f, position.z, 1.0f));
	
	UV = vec2(texcoords);
}
