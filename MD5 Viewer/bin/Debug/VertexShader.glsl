#version 120

attribute vec3 in_position;
attribute vec2 in_texCoord;
attribute vec3 in_normal;
attribute vec3 tangent;
attribute vec3 bitangent;

varying vec2 texCoord;
varying vec3 FragPos;
varying vec3 Normal;

varying vec3 TangentLightDir;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDir;
uniform vec3 viewPos;

varying vec3 OUTTangent;
varying vec3 OUTBitangent;

void main()
{

	gl_Position = projection * view * model * vec4(in_position, 1.0f);
	FragPos = vec3(model * vec4(in_position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * in_normal; 
	texCoord = in_texCoord;	
	
	mat4 normalMatrix = transpose(inverse(model)); 
	vec3 T = vec3(normalize(normalMatrix * vec4(normalize(tangent), 1.0)));
    	vec3 B = vec3(normalize(normalMatrix * vec4(normalize(bitangent), 1.0)));
    	vec3 N = vec3(normalize(normalMatrix * vec4(normalize(in_normal), 1.0)));  
    	
    	T = normalize(vec3(model * vec4(normalize(tangent), 0.0)));
	N = normalize(vec3(model * vec4(in_normal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	B = cross(T, N); 
	
	if (dot(cross(N, T), B) < 0.0f){
			T = T * -1.0f;
		}

    	gl_PointSize = 20.0f;
    	
    	mat3 TBN = transpose(mat3(T, B, N));  
    	TangentLightDir = TBN * lightDir;
    	TangentViewPos  = TBN * viewPos;
    	TangentFragPos  = TBN * FragPos;
    	
    	    	OUTTangent = tangent;
    	OUTBitangent = bitangent;
}
