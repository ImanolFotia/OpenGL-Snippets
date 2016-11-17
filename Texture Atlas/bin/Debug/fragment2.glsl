#version 120

uniform sampler2D sampler;

varying vec2 TexCoords;


void main()
{
	vec4 albedo = texture2D(sampler, TexCoords);
	gl_FragColor = albedo;
}
