vertex prio=1
out vec4 gl_Position(mat4 model, mat4 view, mat4 projection, attr0 vec3 positionAttr)
{
@src
	gl_Position = projection * view * model * vec4(positionAttr, 1);
@endsrc
}

vertex prio=1
vec2 TexCoord(attr2 vec2 textureCoordAttr)
{
@src

	TexCoord = textureCoordAttr;
@endsrc
}


fragment prio=1
vec4 FinalColor(bool useDiffuse, sampler2D texture_diffuse0, vec2 TexCoord, vec3 defaultDiffuse)
{
@src

	if (useDiffuse)
  		FinalColor = texture(texture_diffuse0, TexCoord);
  	else
  		FinalColor = vec4(defaultDiffuse, 1);
@endsrc

}

fragment prio=1
out vec4 FragColor(vec4 FinalColor)
{
@src
	FragColor = FinalColor;
@endsrc
}
