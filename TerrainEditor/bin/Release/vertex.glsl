#version 120

attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 MVP; 
uniform mat4 model;
uniform float time;

varying float color;
varying vec3 FragPos;

uniform float posx;
uniform float posy;

uniform float lastposx;
uniform float lastposy;
uniform float deltaTime;

float height = 1.5;
float frequency = 0.8;
float period = 1/frequency;
float radius = 60;
void main()
{

	vec2 absoluteCurrentPosition = vec2(abs(position.x), abs(position.z));
	vec2 testPostion = vec2(lastposx,lastposy);
	float distanceFromTestingPoint = length(testPostion - absoluteCurrentPosition);
	

	float reductionPosition = (distanceFromTestingPoint);
	float YPos = sin(reductionPosition * period - time * 5 * frequency);
	
	if(distanceFromTestingPoint > radius)
		YPos = 0;
		
	//gl_Position = MVP * vec4(position.x, position.y + YPos * height, position.z, 1.0f); 
	
	gl_Position = MVP * vec4(position, 1.0f);
	
	FragPos = vec3(vec4(position.x, position.y + YPos * height, position.z, 1.0f));
	
	color = FragPos.y;
}
