#version 120

varying float color;
varying vec3 FragPos;

uniform float time;
uniform int ispoint;

void main()
{
	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);//normalize(vec4(mix(0, 1, cos(color - time)),  color, mix(0, 1, sin(color + time)), 1.0));
	if(ispoint == 1)
		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);	
}
