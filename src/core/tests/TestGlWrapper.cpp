#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLWrapperCore>
#include <GLFW/glfw3.h>
#include <boost/test/included/unit_test.hpp>
#include <GL/osmesa.h>
#include <iostream>

using namespace dream::glwrapper;
#ifndef GL_ENABLE_THROW_ON_ERROR
#error GL_ENABLE_THROW_ON_ERROR is disabled
#endif
class TestFixture
{
public:
	TestFixture()
	{
		unsigned int width	= 1000;
		unsigned int height = 1000;

		ctx = OSMesaCreateContext(OSMESA_RGBA, nullptr);
		if (!ctx)
			throw std::runtime_error("Failed to create OSMesa context");

		buffer = new char[width * height * 4 * sizeof(GLubyte)];
		if (!buffer)
			throw std::runtime_error("Failed to allocate buffer");

		if (!OSMesaMakeCurrent(ctx, buffer, GL_UNSIGNED_BYTE, width, height))
			throw std::runtime_error("OSMesaMakeCurrent failed!\n");

		if (!gladLoadGLLoader(
				reinterpret_cast<GLADloadproc>(OSMesaGetProcAddress)))
			throw std::runtime_error("Failed to load opengl functions");
	}
	~TestFixture()
	{
		OSMesaDestroyContext(ctx);
		delete[] buffer;
	}

	char *buffer   = nullptr;
	char *filename = nullptr;
	OSMesaContext ctx;
};

class GLContextState
{
public:
	using GLStateMap = std::map<GLenum, int>;
	static GLContextState &instance()
	{
		static GLContextState state;
		return state;
	}

	const GLStateMap &updateState()
	{
		for (auto &stateVar : mState)
			glGetIntegerv(stateVar.first, &(stateVar.second));
		return mState;
	}

	operator std::string()
	{
		std::string res;
		res.reserve(65542);
		res.append("OpenGL state:\n");
		for (auto &stateVar : mState)
			res.append(glEnumString(stateVar.first) + ": " +
					   std::to_string(stateVar.second) + "\n");
		return res;
	}

private:
	GLContextState()
		: mState({
			  { GL_ARRAY_BUFFER_BINDING, 0 },
			  { GL_ELEMENT_ARRAY_BUFFER_BINDING, 0 },
			  { GL_CURRENT_PROGRAM, 0 },
			  { GL_VERTEX_ARRAY_BINDING, 0 },
			  { GL_TEXTURE_BINDING_1D, 0 },
			  { GL_TEXTURE_BINDING_1D_ARRAY, 0 },
			  { GL_TEXTURE_BINDING_2D, 0 },
			  { GL_TEXTURE_BINDING_2D_ARRAY, 0 },
			  { GL_TEXTURE_BINDING_2D_MULTISAMPLE, 0 },
			  { GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, 0 },
			  { GL_TEXTURE_BINDING_3D, 0 },
			  { GL_TEXTURE_BINDING_BUFFER, 0 },
			  { GL_TEXTURE_BINDING_CUBE_MAP, 0 },
			  { GL_TEXTURE_BINDING_RECTANGLE, 0 },
			  { GL_ACTIVE_TEXTURE, 0 },
			  { GL_FRAMEBUFFER_BINDING, 0 },
			  { GL_READ_FRAMEBUFFER_BINDING, 0 },
			  { GL_DRAW_FRAMEBUFFER_BINDING, 0 },
			  { GL_RENDERBUFFER_BINDING, 0 },
			  //		{GL_DISPATCH_INDIRECT_BUFFER_BINDING, 0},
			  //		{GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, 0},
			  //		{GL_MAX_UNIFORM_BUFFER_BINDINGS, 0},
			  //		{GL_MAX_VERTEX_ATTRIB_BINDINGS, 0},
			  //		{GL_PIXEL_PACK_BUFFER_BINDING, 0},
			  //		{GL_PIXEL_UNPACK_BUFFER_BINDING, 0},
			  //		{GL_PROGRAM_PIPELINE_BINDING, 0},
			  //		{GL_SAMPLER_BINDING, 0},
			  //		{GL_SHADER_STORAGE_BUFFER_BINDING, 0},
			  //		{GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, 0},
			  //		{GL_UNIFORM_BUFFER_BINDING, 0},
			  //		{GL_VERTEX_BINDING_BUFFER, 0},
			  //		{GL_VERTEX_BINDING_DIVISOR, 0},
			  //		{GL_VERTEX_BINDING_OFFSET, 0},
			  //		{GL_VERTEX_BINDING_STRIDE, 0},
			  //		{GL_ACTIVE_PROGRAM, 0},
		  })
	{
	}
	std::map<GLenum, int> mState;
};
std::basic_ostream<char> &operator<<(std::basic_ostream<char> &stream,
									 const std::map<GLenum, int> &map)
{
	for (const auto &v : map)
		stream << "{" << v.first << ", " << v.second << "}" << std::endl;
	return stream;
}

BOOST_FIXTURE_TEST_SUITE(OpengFLWrapperFixture, TestFixture)
const char vertexShaderSource[] =
	"#version 140\n"
	"in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char fragmentShaderSource[] =
	"#version 140\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

BOOST_AUTO_TEST_CASE(TestShaders)
{
	GLVertexShader vs;
	BOOST_TEST(!vs.compileStatus());

	GLFragmentShader fs;
	BOOST_TEST(!fs.compileStatus());

	GLProgram program;
	BOOST_TEST(!program.linkStatus());

	vs.compile(std::string(vertexShaderSource, sizeof(vertexShaderSource)));
	BOOST_TEST(vs.compileStatus(), vs.compilationLog().c_str());

	fs.compile(std::string(fragmentShaderSource, sizeof(fragmentShaderSource)));
	BOOST_TEST(fs.compileStatus(), fs.compilationLog().c_str());

	program.link(vs, fs);
	BOOST_TEST(program.linkStatus(), program.linkageLog().c_str());

	GLProgram fullProgram;
	fullProgram.prepare(vertexShaderSource, fragmentShaderSource);
	BOOST_TEST(fullProgram.linkStatus());
}

BOOST_AUTO_TEST_CASE(TestBindings)
{
	GLContextState &stateProvider = GLContextState::instance();
	auto initialState			  = stateProvider.updateState();
	auto vs						  = std::make_shared<GLVertexShader>();
	auto fs						  = std::make_shared<GLFragmentShader>();
	auto prog					  = std::make_shared<GLProgram>();
	BOOST_REQUIRE((vs->compile(
		std::string(vertexShaderSource, sizeof(vertexShaderSource)))));
	BOOST_REQUIRE((fs->compile(
		std::string(fragmentShaderSource, sizeof(fragmentShaderSource)))));
	BOOST_REQUIRE((prog->link(*vs, *fs)));

	std::vector<std::shared_ptr<GLObject>> objects = {
		std::make_shared<GLVertexArray>(),
		std::make_shared<GLArrayBuffer>(),
		std::make_shared<GLElementArrayBuffer>(),
		prog,
		std::make_shared<GLTexture2D>(),
		std::make_shared<GLTextureUnit>(GL_TEXTURE1),
		std::make_shared<GLTextureUnit>(GL_TEXTURE2),
		std::make_shared<GLTextureUnit>(GL_TEXTURE3),
		std::make_shared<GLRenderBuffer>(),
	};
	for (auto obj : objects)
	{
		{
			GLObjectBinder bind(*obj);
			auto intermediateState = stateProvider.updateState();
			BOOST_TEST(initialState != intermediateState);
			{
				GLObjectBinder bind2(*obj);
				BOOST_TEST(intermediateState == stateProvider.updateState());
			}
			std::cout << std::string(stateProvider) << std::endl;
		}

		BOOST_TEST(initialState == stateProvider.updateState());
		std::cout << std::string(stateProvider) << std::endl << std::endl;
	}

	GLFrameBufferObject fbo;
	GLint readFB = 0;
	GLint drawFB = 0;
	BOOST_TEST(initialState == stateProvider.updateState());
	{
		GLReadDrawFramebufferBinder bind(fbo);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFB);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFB);
		BOOST_TEST(initialState.at(GL_READ_FRAMEBUFFER_BINDING) != readFB);
		BOOST_TEST(initialState.at(GL_DRAW_FRAMEBUFFER_BINDING) != drawFB);
	}
	BOOST_TEST(initialState == stateProvider.updateState());
	{
		GLDrawFramebufferBinder bind(fbo);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFB);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFB);
		BOOST_TEST(initialState.at(GL_READ_FRAMEBUFFER_BINDING) == readFB);
		BOOST_TEST(initialState.at(GL_DRAW_FRAMEBUFFER_BINDING) != drawFB);
	}
	BOOST_TEST(initialState == stateProvider.updateState());
	{
		GLReadFramebufferBinder bind(fbo);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFB);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFB);
		BOOST_TEST(initialState.at(GL_READ_FRAMEBUFFER_BINDING) != readFB);
		BOOST_TEST(initialState.at(GL_DRAW_FRAMEBUFFER_BINDING) == drawFB);
	}
	BOOST_TEST(initialState == stateProvider.updateState());

	// test OpenGL state changes
	glEnable(GL_BLEND);
	{
		GLPushAttribute<GL_ALL_ATTRIB_BITS> pushAll;
		glDisable(GL_BLEND);
	}
	BOOST_TEST(glIsEnabled(GL_BLEND));

	glEnable(GL_DEPTH_TEST);
	{
		GLEnable<false, GL_DEPTH_TEST> dDepth;
		BOOST_TEST(!glIsEnabled(GL_DEPTH_TEST));
	}
	BOOST_TEST(glIsEnabled(GL_DEPTH_TEST));
}

class TestGLProgram : public GLProgram
{
public:
	TestGLProgram(const std::string &name = "TestGLProgram")
		: GLProgram(name)
	{
	}
	GLuint objectId() const
	{
		return mObjectId;
	}
};

BOOST_AUTO_TEST_CASE(TestUniforms)
{
	std::string vsCode = "#version 140\n"
						 "uniform int i1;\n"
						 "uniform ivec2 i2;\n"
						 "uniform ivec3 i3;\n"
						 "uniform ivec4 i4;\n"
						 "uniform float fi1;\n"
						 "uniform vec2 fi2;\n"
						 "uniform vec3 fi3;\n"
						 "uniform vec4 fi4;\n"
						 "uniform uint ui1;\n"
						 "uniform uvec2 ui2;\n"
						 "uniform uvec3 ui3;\n"
						 "uniform uvec4 ui4;\n"
						 "uniform mat4 matrix4;\n"

						 "void main()\n"
						 "{\n"
						 "   gl_Position = vec4((i1 + i2.x + i3.x + i4.x + "
						 "matrix4[0][0]), (fi1 + fi2.x + fi3.x "
						 "+ fi4.x), (ui1 + ui2.x + ui3.x + ui4.x), 1.0);\n"
						 "}\0";

	GLVertexShader vs;
	BOOST_REQUIRE(vs.compile(vsCode));

	GLFragmentShader fs;
	BOOST_REQUIRE(fs.compile(
		std::string(fragmentShaderSource, sizeof(fragmentShaderSource))));

	TestGLProgram prog;
	BOOST_REQUIRE(prog.link(vs, fs));

	BOOST_REQUIRE(prog.getActiveUniforms().size() == 13);

	int i1 = 0, i2[] = { 0, 0 }, i3[] = { 0, 0, 0 }, i4[] = { 0, 0, 0, 0 };
	int ci4[]{ 1, 2, 3, 4 };
	float fi1 = 0, fi2[] = { 0, 0 }, fi3[] = { 0, 0, 0 },
		  fi4[] = { 0, 0, 0, 0 };
	float fci4[]{ 1, 2, 3, 4 };
	unsigned int ui1 = 0, ui2[] = { 0, 0 }, ui3[] = { 0, 0, 0 },
				 ui4[] = { 0, 0, 0, 0 };
	unsigned int uci4[]{ 1, 2, 3, 4 };
	Eigen::Matrix4f mat4, mat4Check = Eigen::Matrix4f::Zero();
	mat4 << 1, 4, 2, 5, 3, 2, 1, 6, 7, 3, 8, 2, 1, 6, 5, 3;

	dream::glwrapper::GLObjectBinder bindProgram(prog);
	prog.setUniform("i1", ci4[0]);
	prog.setUniform("i2", ci4[0], ci4[1]);
	prog.setUniform("i3", ci4[0], ci4[1], ci4[2]);
	prog.setUniform("i4", ci4[0], ci4[1], ci4[2], ci4[3]);
	prog.setUniform("fi1", fci4[0]);
	prog.setUniform("fi2", fci4[0], fci4[1]);
	prog.setUniform("fi3", fci4[0], fci4[1], fci4[2]);
	prog.setUniform("fi4", fci4[0], fci4[1], fci4[2], fci4[3]);
	prog.setUniform("ui1", uci4[0]);
	prog.setUniform("ui2", uci4[0], uci4[1]);
	prog.setUniform("ui3", uci4[0], uci4[1], uci4[2]);
	prog.setUniform("ui4", uci4[0], uci4[1], uci4[2], uci4[3]);
	prog.setUniform("matrix4", mat4);

	GLint i1l = glGetUniformLocation(prog.objectId(), "i1");
	GLint i2l = glGetUniformLocation(prog.objectId(), "i2");
	GLint i3l = glGetUniformLocation(prog.objectId(), "i3");
	GLint i4l = glGetUniformLocation(prog.objectId(), "i4");

	GLint fi1l = glGetUniformLocation(prog.objectId(), "fi1");
	GLint fi2l = glGetUniformLocation(prog.objectId(), "fi2");
	GLint fi3l = glGetUniformLocation(prog.objectId(), "fi3");
	GLint fi4l = glGetUniformLocation(prog.objectId(), "fi4");

	GLint ui1l	= glGetUniformLocation(prog.objectId(), "ui1");
	GLint ui2l	= glGetUniformLocation(prog.objectId(), "ui2");
	GLint ui3l	= glGetUniformLocation(prog.objectId(), "ui3");
	GLint ui4l	= glGetUniformLocation(prog.objectId(), "ui4");
	GLint mat4l = glGetUniformLocation(prog.objectId(), "matrix4");

	glGetUniformiv(prog.objectId(), i1l, &i1);
	glGetUniformiv(prog.objectId(), i2l, i2);
	glGetUniformiv(prog.objectId(), i3l, i3);
	glGetUniformiv(prog.objectId(), i4l, i4);

	glGetUniformfv(prog.objectId(), fi1l, &fi1);
	glGetUniformfv(prog.objectId(), fi2l, fi2);
	glGetUniformfv(prog.objectId(), fi3l, fi3);
	glGetUniformfv(prog.objectId(), fi4l, fi4);

	glGetUniformuiv(prog.objectId(), ui1l, &ui1);
	glGetUniformuiv(prog.objectId(), ui2l, ui2);
	glGetUniformuiv(prog.objectId(), ui3l, ui3);
	glGetUniformuiv(prog.objectId(), ui4l, ui4);

	glGetUniformfv(prog.objectId(), mat4l, mat4Check.data());

	BOOST_TEST(i1 == ci4[0]);
	BOOST_TEST((i2[0] == ci4[0] && i2[1] == ci4[1]));
	BOOST_TEST((i3[0] == ci4[0] && i3[1] == ci4[1] && i3[2] == ci4[2]));
	BOOST_TEST((i4[0] == ci4[0] && i4[1] == ci4[1] && i4[2] == ci4[2] &&
				i4[3] == ci4[3]));

	BOOST_TEST(fi1 == fci4[0]);
	BOOST_TEST((fi2[0] == fci4[0] && fi2[1] == fci4[1]));
	BOOST_TEST((fi3[0] == fci4[0] && fi3[1] == fci4[1] && fi3[2] == fci4[2]));
	BOOST_TEST((fi4[0] == fci4[0] && fi4[1] == fci4[1] && fi4[2] == fci4[2] &&
				fi4[3] == fci4[3]));

	BOOST_TEST(ui1 == uci4[0]);
	BOOST_TEST((ui2[0] == uci4[0] && ui2[1] == uci4[1]));
	BOOST_TEST((ui3[0] == uci4[0] && ui3[1] == uci4[1] && ui3[2] == uci4[2]));
	BOOST_TEST((ui4[0] == uci4[0] && ui4[1] == uci4[1] && ui4[2] == uci4[2] &&
				ui4[3] == uci4[3]));

	for (int i = 0; i < mat4.rows(); i++)
		for (int j = 0; j < mat4.cols(); j++)
			BOOST_TEST(mat4(i, j) == mat4Check(i, j));
}

BOOST_AUTO_TEST_CASE(TextureParameterSetter)
{
	GLTexture2D texture;
	texture.create<2>(0, GLTexture2D::InternalFormat::Rgba,
					  std::array<std::size_t, 2>({ 10, 10 }));
	texture.setParameter(GLTexture2D::ValueMinFilter::MinLinearMipmapLinear);
	texture.setParameter(GLTexture2D::ValueMagFilter::MagNearest);
	texture.setParameter(GLTexture2D::ValueWrapS::MirrorClampToEdgeS);
	texture.setParameter(GLTexture2D::ValueWrapT::MirrorClampToEdgeT);
	texture.generateMipMaps();

	BOOST_TEST(
		texture
			.getParameter<GLTexture2D::TextureParameters::TextureMinFilter>() ==
		GLTexture2D::ValueMinFilter::MinLinearMipmapLinear);
	BOOST_TEST(
		texture
			.getParameter<GLTexture2D::TextureParameters::TextureMagFilter>() ==
		GLTexture2D::ValueMagFilter::MagNearest);
	BOOST_TEST(
		texture.getParameter<GLTexture2D::TextureParameters::TextureWrapS>() ==
		GLTexture2D::ValueWrapS::MirrorClampToEdgeS);
	BOOST_TEST(
		texture.getParameter<GLTexture2D::TextureParameters::TextureWrapT>() ==
		GLTexture2D::ValueWrapT::MirrorClampToEdgeT);

	auto currentState = GLContextState::instance().updateState();
	texture.attach(GL_TEXTURE3);
	BOOST_TEST(currentState == GLContextState::instance().updateState());
}

BOOST_AUTO_TEST_CASE(TestFramebuffer)
{
	GLFrameBufferObject fbo;
	GLReadDrawFramebufferBinder bindFBO(fbo);

	BOOST_TEST(bindFBO.state() != GLReadDrawFramebufferBinder::Complete);

	GLTexture2D texture;
	texture.create<2>(0, GLTexture2D::InternalFormat::Rgba,
					  std::array<std::size_t, 2>({ 100, 100 }));
	fbo.attach(GLFrameBufferObject::AttachmentPoint::Color0, texture);
	GLRenderBuffer rbo;
	rbo.create(100, 100, GL_DEPTH24_STENCIL8);
	fbo.attach(GLFrameBufferObject::AttachmentPoint::DepthStencil, rbo);

	BOOST_TEST(bindFBO.state() == GLReadDrawFramebufferBinder::Complete);
}
BOOST_AUTO_TEST_SUITE_END()
