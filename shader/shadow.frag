#version 330 core
uniform bool diffuseTextureEnabled;
uniform vec3 defaultDiffuse;
uniform sampler2D texture_diffuse1;

uniform bool shadowEnabled;
uniform sampler2D shadow_depth;
uniform int shadowKernelSize;
uniform int shadowKernelOffset;

uniform vec3 lightDirection;


in vec4 LightSpaceCoord;
in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldwPos;

float isInShadow(vec4 lsCoord, float bias)
{
    lsCoord /= lsCoord.w;
    lsCoord = lsCoord * 0.5 + 0.5;
    float depth = texture(shadow_depth, lsCoord.xy).x;
    if((depth + bias) < lsCoord.z){
        return 0;
    }
    return 1;
}

float PCS(int offset = 1, int kernelSize = 0, vec4 lsCoord)
{
    
    kernelSize = (kernelSize < 0)? 0 : kernelSize;
    offset = (offset < 1)?1:offset;
    
    vec4 notmalizedLsCoord = lsCoord / lsCoord.w;
    vec2 depthTexOffset = 1.0f / textureSize(shadow_depth, 0) * offset;
    float shadowStrength = 0;
    for(int i = -kernelSize; i <= kernelSize; i++)
    {
        for (int j = -kernelSize; j <= kernelSize; j++)
        {
            shadowStrength += isInShadow(notmalizedLsCoord + 
                                            vec4(depthTexOffset.x*i, depthTexOffset.y*j, 0, 0), 
                                        0);
        }
    }
    shadowStrength = shadowStrength / ((2*kernelSize + 1) * (2*kernelSize + 1));
    return shadowStrength;
    
}

void main()
{
    vec3 light = -normalize(lightDirection);
    vec3 norm = normalize(Normal);
    if(diffuseTextureEnabled)
        gl_FragColor = texture(texture_diffuse1, TexCoord);
    else
        gl_FragColor = vec4(defaultDiffuse, 1);
    
    float lighting = max(0, dot(norm, light));
    if(shadowEnabled)
        lighting *= PCS(shadowKernelOffset, shadowKernelSize, LightSpaceCoord);
    
    gl_FragColor *= (0.1 + lighting);
    
}
