#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

uniform mat4 mvp;

out vec3 Normal;
out vec2 TextureCoord;
void main()
{
   Normal = normalize(normal);
   TextureCoord = texCoord;
   gl_Position = mvp*vec4(pos,1.0);
}
