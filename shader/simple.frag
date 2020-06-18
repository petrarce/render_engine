#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform bool useDiffuse;
uniform vec3 defaultDiffuse;
void main()
{
	if (useDiffuse)
  		FragColor = texture(texture_diffuse0, TexCoord);
  	else
  		FragColor = vec4(defaultDiffuse, 1);
}
