#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLFW/glfw3.h>
#include <GLWrapperCore>
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
		GLFWwindow *window = glfwCreateWindow(1, 1, "", NULL, NULL);
		if (window == NULL) {
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
BOOST_AUTO_TEST_SUITE_END()
