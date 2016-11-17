#version 120

attribute vec3 position;
attribute vec2 UV;
attribute vec2 LMUV;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec2 TexCoords;
varying vec2 LightMapTexCoords;
varying vec3 FragPos;
varying vec3 Normal;
uniform vec3 viewPos;
uniform mat4 normalMatrix;
vec3 lightPos = vec3(-0.191195, -11.722448, -5.879405);


varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;
   
varying vec3 OUTTangent;
varying vec3 OUTBitangent;

vec3 o_normal;

// constant that are used to adjust lighting
const float C1 = 0.429043;
const float C2 = 0.511664;
const float C3 = 0.743125;
const float C4 = 0.886227;
const float C5 = 0.247708;

// scale for restored amount of lighting
float u_scaleFactor = 1;

// coefficients of spherical harmonics and possible values
vec3 u_L00 = vec3(0.79, 0.44, 0.54);
vec3 u_L1m1= vec3(0.39, 0.35, 0.60);
vec3 u_L10= vec3(-0.34, -0.18, -0.27);
vec3 u_L11= vec3(-0.29, -0.06, 0.01);
vec3 u_L2m2= vec3(-0.26, -0.22, -0.47);
vec3 u_L2m1= vec3(-0.11, -0.05, -0.12);
vec3 u_L20= vec3(-0.16, -0.09, -0.15);
vec3 u_L21= vec3(0.56, 0.21, 0.14);
vec3 u_L22=vec3(0.21, -0.05, -0.30);

///////////////////////////////////////////

// function restores lighting at a vertex from normal and
// from coefficient of spherical harmonics
vec3 sphericalHarmonics(vec3 N)
{
   return
      // band 0, constant value, details of lowest frequency
      C4 * u_L00 +

      // band 1, oriented along main axes
      2.0 * C2 * u_L11 * N.x +
      2.0 * C2 * u_L1m1 * N.y +
      2.0 * C2 * u_L10 * N.z +

      // band 2, values depend on multiple axes, higher frequency details
      C1 * u_L22 * (N.x * N.x - N.y * N.y) +
      C3 * u_L20 * N.z * N.z - C5 * u_L20 +
      2.0 * C1 * u_L2m2 * N.x * N.y +
      2.0 * C1 * u_L21 * N.x * N.z +
      2.0 * C1 * u_L2m1 * N.y * N.z;
}

varying vec3 color;
varying mat3 oTBN;
void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	TexCoords = UV;
	LightMapTexCoords = LMUV;
	FragPos = vec3(model * vec4(position, 1.0f));
	
	color = sphericalHarmonics(vec3(normalMatrix * vec4(normal, 1.0))) * 2.0;
	
    	vec3 T = vec3(normalize(normalMatrix * vec4(normalize(tangent), 1.0)));
    	vec3 B = vec3(normalize(normalMatrix * vec4(normalize(bitangent), 1.0)));
    	vec3 N = vec3(normalize(normalMatrix * vec4(normalize(normal), 1.0)));   
    	
    	
    	
    	T = normalize(vec3(model * vec4(tangent, 0.0)));
	N = normalize(vec3(model * vec4(normal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	B = cross(T, N); 
    
    	mat3 TBN = transpose(mat3(T, B, N));  
    	TangentLightPos = TBN * lightPos;
    	TangentViewPos  = TBN * viewPos;
    	TangentFragPos  = TBN * FragPos;
    	oTBN=TBN;
    	
    	OUTTangent = tangent;
    	OUTBitangent = bitangent;
    	Normal = normal;
    	
}

