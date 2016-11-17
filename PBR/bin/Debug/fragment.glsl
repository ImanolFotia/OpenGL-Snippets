#version 120

varying vec2 texCoords;
varying vec3 Normal;
varying vec3 FragPos;
varying vec3 Position;
uniform float Metallic;
uniform float Roughness = 64;
uniform vec3 viewPos;
uniform samplerCube cubemap;
uniform sampler2D normalMap;
uniform sampler2D AlbedoMap;
uniform sampler2D noise;
uniform sampler2D Height;
uniform float time;
uniform float samplingpattern;
float offset = 0.5 / Roughness; 
float omin = 0.5f / 24.0f;
float omax = 0.5f / 10000.0f;

vec2 sampling = vec2(gl_FragCoord.x/samplingpattern, gl_FragCoord.y/samplingpattern);

vec3 bpcem (in vec3 v, vec3 Emax, vec3 Emin, vec3 Epos)
{	
	vec3 nrdir = normalize(v);
	vec3 rbmax = (Emax - FragPos)/nrdir;
	vec3 rbmin = (Emin - FragPos)/nrdir;
	
	vec3 rbminmax;
	rbminmax.x = (nrdir.x>0.0)?rbmax.x:rbmin.x;
	rbminmax.y = (nrdir.y>0.0)?rbmax.y:rbmin.y;
	rbminmax.z = (nrdir.z>0.0)?rbmax.z:rbmin.z;		
	float fa = min(min(rbminmax.x, rbminmax.y), rbminmax.z);
	vec3 posonbox = FragPos + nrdir * fa;
	return posonbox - Epos;
}

vec3 sampleCubeMap()
{
	vec3 I = normalize(Position - viewPos);
    	vec3 R = reflect(I, normalize(Normal));
    	
offset = clamp(offset, omax, omin);

    	vec3 offsets[9] = vec3[](
        vec3(-offset, offset, offset),  // top-left
        vec3(0.0f,    offset, offset),  // top-center
        vec3(offset,  offset, offset),  // top-right
        vec3(-offset, 0.0f, offset),    // center-left
        vec3(0.0f,    0.0f, 0.0f),    // center-center
        vec3(offset,  0.0f, -offset),    // center-right
        vec3(-offset, -offset, -offset), // bottom-left
        vec3(0.0f,    -offset, -offset), // bottom-center
        vec3(offset,  -offset, -offset)  // bottom-right    
    );

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
    
    vec3 sampleTex[9];
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(textureCube(cubemap, R.xyz + offsets[i]));
        col += sampleTex[i] * kernel[i];
    }
    
    return col;
}

vec3 BoxMax = vec3(200,200,200);
vec3 BoxMin = vec3(-200,0,-200);

varying vec3 tanViewPos;
varying vec3 tanFragPos;

float height_scale = 0.1;

vec2 ParallaxMapping(in vec2 texCoords, in vec3 viewDir, inout float depth)
{ 
    // number of depth layers
    const float minLayers = 10;
    const float maxLayers = 30;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture2D(Height, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture2D(Height, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture2D(Height, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    depth = currentLayerDepth;
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}
float near = 1.0; 
float far  = 1000.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
uniform mat4 projection;
uniform mat4 view;
void main()
{
	vec3 VD = normalize(tanFragPos - tanViewPos);
	float d;
	vec2 coords = ParallaxMapping(texCoords, VD, d);
	/*
	if(coords.y < 0.0 || coords.y > 1.0 || coords.x > 1.0 || coords.x < 0.0)
		discard;
	*/
	vec3 normaltex =  normalize(texture2D(normalMap, vec2(coords.x, coords.y)).rbg * 2.0 - 1.0);

	vec3 Albedotex = texture2D(AlbedoMap, coords).rgb;
	float noiseTex = texture2D(noise, sampling).r;
	
	if(noiseTex == 0.0)
		discard;
	
    	float diff = max(dot(normaltex, vec3(0.8, 1.0, 0.3)), 0.0);
    	vec3 diffuse = diff * Albedotex;
    	
    	vec3 viewDir = normalize(viewPos - FragPos);
    	vec3 halfwayDir = normalize(vec3(0.8, 1.0, 0.3) + viewDir);  
    	float spec = pow(max(dot(normaltex, halfwayDir), 0.0), 128.0);
    	

	vec3 dir = normalize(FragPos - viewPos);
	vec3 rdir = reflect(-viewDir, normaltex);
	vec3 reflectedcube = textureCube(cubemap, bpcem(rdir, BoxMax, BoxMin, vec3(0.0, 100.0, 0.0))).rgb;
	vec3 lighting = diffuse + spec/* + reflectedcube * 0.5*/;
	float factor = clamp(dot(-dir, normaltex), 0.0, 0.9);
	/*
	vec3 R = reflect(viewDir, normaltex);
	reflectedcube = textureCube(cubemap, R).rgb;*/
	d = (view * vec4(vec3(d), 1.0)).z;
	vec4 clipspace = projection * vec4(vec3(d), 1.0);
	vec3 NDC = clipspace.xyz / clipspace.w;
	vec3 ss = NDC * 0.5 + 0.5;
	//gl_FragDepth = gl_FragCoord.z + vec3(MVP * vec4(ss, 1.0)).z;
		
	gl_FragColor.rgb = vec3(gl_FragCoord.z + vec3(projection * vec4(vec3(d), 1.0)).z);// = mix(reflectedcube, lighting, 1.0) * noiseTex;//mix(lighting, reflectedcube, clamp(1 - factor, 0.0, 0.4));
}














