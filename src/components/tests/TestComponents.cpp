
#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLWrapperCore>
#include <GLComponents>
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <GL/osmesa.h>
#include <molecular/util/Hash.h>

using namespace dream::glwrapper;
using namespace dream::components;
using namespace molecular::util;

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

BOOST_FIXTURE_TEST_SUITE(OpengGLComponentsFixture, TestFixture)
std::string program =
	"vertex prio=0\n"
	"out vec4 gl_Position(int val1, mat4 val2)\n"
	"{\n"
	"@src\n"
	"	gl_Position = vec4(val1, val2[0][0], val2[0][1], val2[0][1]);\n"
	"@endsrc"
	"}\n"
	"fragment prio=0\n"
	"out vec4 fragColor(float val3)\n"
	"{\n"
	"@src\n"
	"	fragColor = vec4(val3, val3, val3, val3);\n"
	"@endsrc"
	"}\n";

BOOST_AUTO_TEST_CASE(TestProgramGenerator)
{
	dream::components::Scope rootScope;
	rootScope.Set(molecular::util::HashUtils::MakeHash("val1"),
				  Uniform<int>(10));
	rootScope.Set(molecular::util::HashUtils::MakeHash("val2"),
				  Uniform<Eigen::Matrix4f>(Eigen::Matrix4f::Identity()));
	rootScope.Set(molecular::util::HashUtils::MakeHash("val3"),
				  Uniform<float>(15.f));
	rootScope.Set(molecular::util::HashUtils::MakeHash("fragColor"), Output());
	rootScope.Set(molecular::util::HashUtils::MakeHash("gl_Position"),
				  Output());

	GLMolecularProgramGenerator &generator =
		GLMolecularProgramGenerator::instance();
	generator.deploy(program);
	GLMolecularProgram progContainer;
	progContainer.generate(rootScope);
	progContainer.prepare(rootScope);
}

BOOST_AUTO_TEST_SUITE_END()
