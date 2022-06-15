#include <GLAssetManager.hpp>
#include <GLWrapperCore>
#include <GLMolecularProgram.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
namespace dream
{
namespace renderer
{

template <>
std::shared_ptr<glwrapper::GLTexture2D>
GLAssetManager<glwrapper::GLTexture2D>::loadAsset(
	const std::string &path, glwrapper::GLTexture2D::InternalFormat format)
{
	std::cout << "load texture template" << std::endl;
	int width = 0, height = 0, components = 0;
	unsigned char *data =
		stbi_load(path.c_str(), &width, &height, &components, 0);
	if (!data)
		return nullptr;
	std::vector<char> dataBuf(width * height * components);
	std::copy(data, data + dataBuf.size(), dataBuf.begin());
	free(data);
	auto texture = std::make_shared<glwrapper::GLTexture2D>();
	glwrapper::GLTexture2D::DataFormat dataFormat;
	if (components == 1)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRed;
	else if (components == 2)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRg;
	else if (components == 3)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRgb;
	else if (components == 4)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRgba;
	else
	{
		assert(0 || "unexpected number of components");
		return nullptr;
	}
	std::array<size_t, 2> size;
	size[0] = width;
	size[1] = height;
	texture->create<char, 2>(dataBuf, 0, format, dataFormat,
							 glwrapper::GLTexture2D::DataType::UByte, size);
	texture->generateMipMaps();
	return texture;
}

template <>
std::shared_ptr<components::GLMolecularProgram>
GLAssetManager<components::GLMolecularProgram>::loadAsset(
	const components::Scope &scope)
{
	auto program = std::make_shared<components::GLMolecularProgram>();
	program->generate(scope);
	return program;
}

} // namespace renderer
} // namespace dream
