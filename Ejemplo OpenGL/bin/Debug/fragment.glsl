#version 110

uniform float time;

uniform vec2 mousepos;

varying vec2 line;

void main()
{
	if(line.x* 800.0 < mousepos.x + .5 && line.x * 800.0 > mousepos.x - .5)
	gl_FragColor.rgb = vec3(1.0, 0.0, 0.0);
	else
	gl_FragColor.rgb = vec3(0.0, 1.0, 0.0); 
	
}
