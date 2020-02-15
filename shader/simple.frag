#version 330 core
in vec3 Normal;
in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 defaultDiffuse;
uniform vec3 defaultSpecular;
uniform bool useDiffuse;
uniform bool useSpecular;


void main()
{
	if(useDiffuse){
		FragColor = vec4(vec3(texture(texture_diffuse1, TextureCoord)),1.0f);
		return;
	}
	FragColor = vec4(Normal, 1.0f);
}
