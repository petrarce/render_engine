#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLWrapperCore>
#include <GLFW/glfw3.h>
#include <boost/test/included/unit_test.hpp>
#include <iostream>

using namespace glwrapper::core;

class TestFixture
{
public:
	TestFixture()
	{
		BOOST_TEST_MESSAGE("Setup opengl");

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		// --------------------
		GLFWwindow *window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
		if (window == nullptr) {
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(
			window, [](GLFWwindow *window, int width, int height) {
				// make sure the viewport matches the new window dimensions;
				// note that width and height will be significantly larger than
				// specified on retina displays.
				glViewport(0, 0, width, height);
			});

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			throw std::runtime_error("Failed to initialize GLAD");
		}
	}
	~TestFixture()
	{
		BOOST_TEST_MESSAGE("Teardown opengl");
		glfwTerminate();
	}
};

std::map<GLenum, int> getGlBindings()
{
	std::map<GLenum, int> state = {
		{GL_ARRAY_BUFFER_BINDING, 0},
		{GL_DISPATCH_INDIRECT_BUFFER_BINDING, 0},
		{GL_DRAW_FRAMEBUFFER_BINDING, 0},
		{GL_ELEMENT_ARRAY_BUFFER_BINDING, 0},
		{GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, 0},
		{GL_MAX_UNIFORM_BUFFER_BINDINGS, 0},
		{GL_MAX_VERTEX_ATTRIB_BINDINGS, 0},
		{GL_PIXEL_PACK_BUFFER_BINDING, 0},
		{GL_PIXEL_UNPACK_BUFFER_BINDING, 0},
		{GL_PROGRAM_PIPELINE_BINDING, 0},
		{GL_READ_FRAMEBUFFER_BINDING, 0},
		{GL_RENDERBUFFER_BINDING, 0},
		{GL_SAMPLER_BINDING, 0},
		{GL_SHADER_STORAGE_BUFFER_BINDING, 0},
		{GL_TEXTURE_BINDING_1D, 0},
		{GL_TEXTURE_BINDING_1D_ARRAY, 0},
		{GL_TEXTURE_BINDING_2D, 0},
		{GL_TEXTURE_BINDING_2D_ARRAY, 0},
		{GL_TEXTURE_BINDING_2D_MULTISAMPLE, 0},
		{GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, 0},
		{GL_TEXTURE_BINDING_3D, 0},
		{GL_TEXTURE_BINDING_BUFFER, 0},
		{GL_TEXTURE_BINDING_CUBE_MAP, 0},
		{GL_TEXTURE_BINDING_RECTANGLE, 0},
		{GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, 0},
		{GL_UNIFORM_BUFFER_BINDING, 0},
		{GL_VERTEX_ARRAY_BINDING, 0},
		{GL_VERTEX_BINDING_BUFFER, 0},
		{GL_VERTEX_BINDING_DIVISOR, 0},
		{GL_VERTEX_BINDING_OFFSET, 0},
		{GL_VERTEX_BINDING_STRIDE, 0},
	};
	for (auto &binding : state)
		glGetIntegerv(binding.first, &binding.second);
	return state;
}

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &stream,
									 const std::map<GLenum, int> &map)
{
	for (const auto &v : map)
		stream << "{" << v.first << ", " << v.second << "}" << std::endl;
	return stream;
}

BOOST_FIXTURE_TEST_SUITE(OpengFLWrapperFixture, TestFixture)
const char vertexShaderSource[] =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char fragmentShaderSource[] =
	"#version 330 core\n"
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

	GLGeometryShader gs;
	BOOST_TEST(!gs.compileStatus());

	GLProgram program;
	BOOST_TEST(!program.linkStatus());

	vs.compile(std::string(vertexShaderSource, sizeof(vertexShaderSource)));
	BOOST_TEST(vs.compileStatus());

	fs.compile(std::string(fragmentShaderSource, sizeof(fragmentShaderSource)));
	BOOST_TEST(fs.compileStatus());

	gs.compile(std::string(fragmentShaderSource, sizeof(fragmentShaderSource)));
	BOOST_TEST(gs.compileStatus());

	program.link(vs, fs);
	BOOST_TEST(program.linkStatus());
}

BOOST_AUTO_TEST_CASE(TestBindings)
{
	auto initialState = getGlBindings();
	std::vector<std::shared_ptr<GLObject>> objects = {
		std::make_shared<GLVertexArray>(),
		std::make_shared<GLArrayBuffer>(),
		std::make_shared<GLElementArrayBuffer>(),
	};
	for (auto obj : objects) {
		BOOST_TEST(initialState == getGlBindings());
		{
			GlObjectBinder bind(*obj);
			BOOST_TEST(initialState != getGlBindings());
		}
		BOOST_TEST(initialState == getGlBindings());
	}
}
class TestGLProgram : public GLProgram
{
public:
	TestGLProgram(const std::string& name = "TestGLProgram") : GLProgram(name) {}
	GLuint objectId() const { return mObjectId; }
};

BOOST_AUTO_TEST_CASE(TestUniforms)
{
	std::string vsCode =
			"#version 330 core\n"
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
			
			"void main()\n"
			"{\n"
			"   gl_Position = vec4((i1 + i2.x + i3.x + i4.x), (fi1 + fi2.x + fi3.x + fi4.x), (ui1 + ui2.x + ui3.x + ui4.x), 1.0);\n"
			"}\0";

	GLVertexShader vs;
	BOOST_REQUIRE(vs.compile(vsCode));

	GLFragmentShader fs;
	BOOST_REQUIRE(fs.compile(std::string(fragmentShaderSource, sizeof(fragmentShaderSource))));

	TestGLProgram prog;
	BOOST_REQUIRE(prog.link(vs, fs));

	int i1 = 0, i2[] = {0, 0}, i3[] = {0, 0, 0}, i4[] = {0, 0, 0, 0};
	int ci4[] {1, 2, 3, 4};
	float fi1 = 0, fi2[] = {0, 0}, fi3[] = {0, 0, 0}, fi4[] = {0, 0, 0, 0};
	float fci4[] {1, 2, 3, 4};
	unsigned int ui1 = 0, ui2[] = {0, 0}, ui3[] = {0, 0, 0}, ui4[] = {0, 0, 0, 0};
	unsigned int uci4[] {1, 2, 3, 4};
	
	prog.use();
	prog.setUniform1("i1", ci4[0]);
	prog.setUniform2("i2", ci4[0], ci4[1]);
	prog.setUniform3("i3", ci4[0], ci4[1], ci4[2]);
	prog.setUniform4("i4", ci4[0], ci4[1], ci4[2], ci4[3]);
	prog.setUniform1("fi1", fci4[0]);
	prog.setUniform2("fi2", fci4[0], fci4[1]);
	prog.setUniform3("fi3", fci4[0], fci4[1], fci4[2]);
	prog.setUniform4("fi4", fci4[0], fci4[1], fci4[2], fci4[3]);
	prog.setUniform1("ui1", uci4[0]);
	prog.setUniform2("ui2", uci4[0], uci4[1]);
	prog.setUniform3("ui3", uci4[0], uci4[1], uci4[2]);
	prog.setUniform4("ui4", uci4[0], uci4[1], uci4[2], uci4[3]);

	
	GLint i1l = glGetUniformLocation(prog.objectId(), "i1");
	GLint i2l = glGetUniformLocation(prog.objectId(), "i2");
	GLint i3l = glGetUniformLocation(prog.objectId(), "i3");
	GLint i4l = glGetUniformLocation(prog.objectId(), "i4");
	
	GLint fi1l = glGetUniformLocation(prog.objectId(), "fi1");
	GLint fi2l = glGetUniformLocation(prog.objectId(), "fi2");
	GLint fi3l = glGetUniformLocation(prog.objectId(), "fi3");
	GLint fi4l = glGetUniformLocation(prog.objectId(), "fi4");
	
	GLint ui1l = glGetUniformLocation(prog.objectId(), "ui1");
	GLint ui2l = glGetUniformLocation(prog.objectId(), "ui2");
	GLint ui3l = glGetUniformLocation(prog.objectId(), "ui3");
	GLint ui4l = glGetUniformLocation(prog.objectId(), "ui4");
	
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
	
	BOOST_TEST(i1 == ci4[0]);
	BOOST_TEST((i2[0] == ci4[0] && i2[1] == ci4[1]));
	BOOST_TEST((i3[0] == ci4[0] && i3[1] == ci4[1] && i3[2] == ci4[2]));
	BOOST_TEST((i4[0] == ci4[0] && i4[1] == ci4[1] && i4[2] == ci4[2] && i4[3] == ci4[3]));
	
	BOOST_TEST(fi1 == fci4[0]);
	BOOST_TEST((fi2[0] == fci4[0] && fi2[1] == fci4[1]));
	BOOST_TEST((fi3[0] == fci4[0] && fi3[1] == fci4[1] && fi3[2] == fci4[2]));
	BOOST_TEST((fi4[0] == fci4[0] && fi4[1] == fci4[1] && fi4[2] == fci4[2] && fi4[3] == fci4[3]));
	
	BOOST_TEST(ui1 == uci4[0]);
	BOOST_TEST((ui2[0] == uci4[0] && ui2[1] == uci4[1]));
	BOOST_TEST((ui3[0] == uci4[0] && ui3[1] == uci4[1] && ui3[2] == uci4[2]));
	BOOST_TEST((ui4[0] == uci4[0] && ui4[1] == uci4[1] && ui4[2] == uci4[2] && ui4[3] == uci4[3]));

}
BOOST_AUTO_TEST_SUITE_END()
