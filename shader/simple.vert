#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 mvp;

out vec3 Normal;
out vec2 TextureCoord;
out vec3 WorldPos;
out vec3 Tangent;
out vec3 Bitangent;
void main()
{
   Normal = normalize(mat3(transpose(inverse(model))) * normal) ;
   if(1 - abs(dot(Normal, vec3(0,1,0))) < 0.001){
        Tangent = normalize(cross(Normal, vec3(1,0,0)));
        Bitangent = normalize(cross(Normal, Tangent));
   } else {
        Tangent = normalize(cross(Normal, vec3(0,1,0)));
        Bitangent = normalize(cross(Normal, Tangent));
   }
   TextureCoord = texCoord;
   WorldPos = vec3(model * vec4(pos, 1));
   gl_Position = mvp*vec4(pos,1.0);
}
