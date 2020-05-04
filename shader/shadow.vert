#version 330 core
layout (location = 0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightViewProjection;

out vec4 LightSpaceCoord;
out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;

void main()
{
    WorldPos = vec3(model * vec4(pos, 1));
    gl_Position = projection * view * vec4(WorldPos,1);
    LightSpaceCoord = lightViewProjection * vec4(WorldPos, 1);
    TexCoord = texCoord;
    Normal = normal;
}
