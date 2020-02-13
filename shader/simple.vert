#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

uniform mat4 mvp;

out vec3 nrm;
out vec3 color;
void main()
{
   nrm = normalize(normal);

   gl_Position = mvp*vec4(pos,1.0);
}
