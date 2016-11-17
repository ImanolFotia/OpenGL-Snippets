#version 120

varying vec2 UV;
uniform sampler2D texture;

void main()
{
	gl_FragColor.rgb = texture2D(texture, -UV).rgb;
}
/*
rgb x
gbr x
bgr x
rbg x
grb o
*/