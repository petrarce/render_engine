vertex prio=1
out vec3 gl_Position(mat4 projection, mat4 view, mat4 model, attr0 vec3 positionAttr)
{
@src
	gl_Position = projection * view * model * vec4(positionAttr, 1);
@endsrc
}

vertex prio=2
vec2 TexCoord(attr1 vec2 texCoord)
{
@src
	TexCoord = texCoord;
@endsrc
}

fragment prio=3
vec4 FinalColor(vec2 TexCoord, sampler2D diffuse_texture0, bool useDiffuse, vec3 defaultDiffuse)
{
@src
	if (useDiffuse)
		FinalColor = texture(diffuse_texture0, TexCoord);
	else 
		FinalColor = vec4(defaultDiffuse, 1);
@endsrc
}	

fragment prio=4
out vec4 FragColor(vec4 FinalColor)
{
@src
	FragColor = FinalColor;
@endsrc
}
