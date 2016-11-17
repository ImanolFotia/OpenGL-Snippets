#version 120

varying vec3 COL;

void main()
{
	gl_FragColor = vec4(COL, 1.0f);
}
