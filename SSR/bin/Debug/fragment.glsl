#version 120

varying vec2 texCoords;
varying vec3 Normal;
uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

vec3 SSR()
{
	return vec3(1.0);
}

void main()
{
	vec4 position_depth = texture2D(gPositionDepth, texCoords);
	vec3 normal = texture2D(gNormal, texCoords).rgb;
	vec4 albedo = texture2D(gAlbedo, texCoords);



	gl_FragColor = vec4(normalize(Normal), 1.0);

}