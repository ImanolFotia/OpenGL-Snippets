#version 120

uniform sampler2D sampler;
uniform sampler2D LightMapsampler;
uniform sampler2D normalSampler;

uniform vec3 viewPos;

varying vec2 TexCoords;
varying vec2 LightMapTexCoords;

uniform bool lightmap;

const float offset = 1.0 / 300; 
vec3 lightPos = vec3(-0.191195, -11.722448, -5.879405);
vec3 lightColor = vec3(1,1,1);
vec4 FragColor;

varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;

varying vec3 OUTTangent;
varying vec3 OUTBitangent;


float time;

void my_func()
{
	vec3 nothing = vec3(1.0);
}



float time2;

void my_func2()
{
	vec3 nothing = vec3(1.0);
}

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
	vec4 Color = texture2D(sampler, TexCoords);
	vec4 LightMap = texture2D(LightMapsampler, LightMapTexCoords);
	vec3 NormalTex = normalize(texture2D(normalSampler, TexCoords).rgb * 2.0 - 1.0);
	vec3 NorTexTBN = NormalTex * oTBN;
	vec3 o = sphericalHarmonics(NorTexTBN) * 5.0;
	
    	// Diffuse
    	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    	float diff = max(dot(lightDir, NormalTex), 0.0);
    	vec3 diffuse = diff * Color.rgb;
    	// Specular
    	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    	vec3 halfwayDir = normalize(lightDir + viewDir);  
    	float spec = pow(max(dot(NormalTex, halfwayDir), 0.0), 32.0);
    	vec3 specular = .5 * spec * vec3(1);
    
	LightMap.rgb = pow(LightMap.rgb*4, vec3(1.0/1.3));
	vec3 ambient = LightMap.r * Color.rgb;
	FragColor = vec4(ambient + diffuse*LightMap.r + specular*LightMap.r, 1.0f);
	
	if(lightmap)
	gl_FragColor = LightMap * Color; //FragColor;//mix(vec4(0,0,0,1), FragColor, LightMap.r);
	else	
	gl_FragColor = LightMap * Color; //FragColor;
	
	gl_FragColor.rgb = o; 
	
	gl_FragColor.a = 1.0;
}

 

