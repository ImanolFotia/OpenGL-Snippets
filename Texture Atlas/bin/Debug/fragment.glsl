#version 120

uniform sampler2D sampler;

varying vec2 TexCoords;

uniform float x;
uniform float y;

void main()
{
	float uCoord = (TexCoords.x/4) + x;
	float vCoord = (TexCoords.y/4) + y;
	
	vec2 AtlasCoords = vec2(uCoord, vCoord);

	vec4 albedo = texture2D(sampler, AtlasCoords);
	//if(albedo.a < 0.001)
	//	discard;
	gl_FragColor = albedo;
	gl_FragColor.a = albedo.r;
}
