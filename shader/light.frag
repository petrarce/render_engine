#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frame;
uniform sampler2D depth_texture;
uniform int numPixels;

void main()
{
    float depth = texture(depth_texture, TexCoords).x;
    depth = depth * 2 - 1;  
    vec2 texDirection = vec2(1,1) * (1.0f / textureSize(frame, 0));
    vec3 color = vec3(0);
    for(int i = 0; i <= numPixels * 2; i++) {
        color += texture(frame, TexCoords + vec2(texDirection.x * (i - numPixels), 0)).rgb;
    }
    for(int j = 0; j <= numPixels * 2; j++) {
        if(j == numPixels){
            continue;
        }
        color += texture(frame, TexCoords + vec2(0, texDirection.x * (j - numPixels))).rgb;
    }

    color /= (2*numPixels + 1)*(2*numPixels + 1);
    FragColor = vec4(color.xyz, 1.0);
} 
