#version 330 core
in vec3 Normal;
in vec2 TextureCoord;
in vec3 WorldPos;
in vec3 Tangent;
in vec3 Bitangent;


out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform vec3 defaultDiffuse;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 camPosition;

//PHONG BLINNPHONG GGX
uniform int lightingModel;

//ambient lighting factor
uniform float ambFactor;
//Phong and BlinnPhong parameters
uniform float specFactor;
uniform float diffFactor;
//ggx configs
uniform float rghness;
uniform float frnlFactor;
uniform float metalicness;

//mode: ALDEBO, NORMALMAP ROUGHNESS HEIGHT 
uniform int mode;
//maps that are present
uniform bool hasNormalMap;
uniform bool hasDiffuseMap;
uniform bool hasSpecularMap;


//lighting models
#define	PHONG 0
#define	BLINNPHONG 1
#define	GGX 2

//rendering mode
#define ALDEBO 0
#define NORMALMAP 1
#define ROUGHNESS 2
#define HEIGHT 3

const float PI = 3.14159265359;

float computeDiffuse(vec3 light, vec3 normal){
	return max(0, dot(light, normal));
}

float specPhong(vec3 light, vec3 normal, vec3 view){
	vec3 h = normalize(light + view);
	return pow(max(0, dot(normal, h)), 32);
}

float specBlinnPhong(vec3 light, vec3 normal, vec3 view){
	vec3 refl = normalize(reflect(-light, normal));
	return pow(max(0, dot(view, refl)), 32);
}

float G(vec3 x, vec3 n, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float NdotV = max(0, dot(x,n));
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float fresnelSchlick(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}


vec3 phong(vec3 light, vec3 normal, vec3 view, vec3 LColor, vec3 FColor){
	float ambient = ambFactor;
	float diffuse = diffFactor * computeDiffuse(light, normal);
	float specular = specFactor * specPhong(light, normal, view);
	return FColor * LColor * (ambient + diffuse + specular);
}

vec3 blinnPhong(vec3 light, vec3 normal, vec3 view, vec3 LColor, vec3 FColor){
	float ambient = ambFactor;
	float diffuse = diffFactor * computeDiffuse(light, normal);
	float specular = specFactor * specBlinnPhong(light, normal, view);
	return FColor * LColor * (ambient + diffuse + specular);
}

vec3 ggx(vec3 L, vec3 N, vec3 V, float roughness, float F0, vec3 LColor, vec3 FragColor){
    vec3 H = normalize(V + L);
    vec3 radiance = LColor;

    float NdotL = max(0, dot(N, L));
    float NdotV = max(0, dot(N, V));
    float NdotH = max(0, dot(N, H));
    float NDF = DistributionGGX(NdotH, roughness);   
    float G   = GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);      
    float F    = fresnelSchlick(NdotV, F0);
       
    float nominator    = NDF * G * F; 
    float denominator = 4 * NdotV * NdotL + 0.001; // 0.001 to prevent divide by zero.
      

    float specular = nominator / denominator;

    float kS = F;

    float kD = 1 - kS;
    kD *= (1 - metalicness);

    vec3 Lo = (kD * FragColor / PI + specular * vec3(1)) * radiance * NdotL;

    return ambFactor * FragColor + Lo;
}


void main()
{
	vec3 light = normalize(lightPosition - WorldPos);
	vec3 view = normalize(camPosition - WorldPos);
	vec3 normal = normalize(Normal);

	if(hasNormalMap){
		mat3 trsf = mat3(Tangent, Bitangent, Normal);
		normal = vec3(texture(texture_normal1, TextureCoord)) * 2 - vec3(1.0f);
		normal = normalize(trsf * vec3(texture(texture_normal1, TextureCoord)));
	} 

	vec4 aldebo = vec4(defaultDiffuse, 1.0f);
	if(hasDiffuseMap){
		aldebo = vec4(vec3(texture(texture_diffuse1, TextureCoord)),1.0f);
	}

	switch(mode){
		case NORMALMAP:
		vec3 normalColor = (normal + vec3(1.0f))*0.5;
		FragColor = vec4(normalColor, 1.0f);
		return;

		case ALDEBO:
		default:
		FragColor = aldebo;
		break;		
	}

	float amb = ambFactor;
	float diffs = 0;
	float spec = 0;
	switch(lightingModel){

		case BLINNPHONG:
		FragColor = vec4(blinnPhong(light, normal, view, lightColor, vec3(FragColor)), 1.0f);
		break;

		case GGX:
		FragColor = vec4(ggx(light, normal, view, rghness, frnlFactor, lightColor, vec3(FragColor)), 1.0f);
		break;

		case PHONG:
		default:
		FragColor = vec4(phong(light, normal, view, lightColor, vec3(FragColor)), 1.0f);
		break;
	}

}
