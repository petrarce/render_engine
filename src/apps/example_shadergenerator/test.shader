vertex
vec2 texCoord(vec2 textureCoordAttribute)
{
	texCoord = textureCoordAttribute;
}

vertex 
float somevar()
{
	somevar = 0.3;
}

fragment prio=2
out vec4 fragmentColor(sampler2D diffuseTexture, vec2 texCoord, float somevar)
{
	fragmentColor = texture(diffuseTexture, texCoord);
}
