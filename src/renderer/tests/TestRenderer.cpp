
#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLWrapperCore>
#include <GLComponents>
#include <GLRenderableObject.hpp>
#include <boost/test/included/unit_test.hpp>
#include <iostream>

using namespace dream::renderer;

class TestGLRenderableObject : public GLRenderableObject
{
public:
	TestGLRenderableObject(const std::string &name = "TestGLRenderableObject")
		: GLRenderableObject(name)
	{
		mRenderFunction =
			std::make_shared<dream::components::GLMultipleCaleeRenderFunction>(
				dream::components::GLMultipleCaleeRenderFunction::CaleeArray());
	}
	std::string printTree(const std::string &prefix) const
	{
		std::string result;
		for (auto c : mChildren)
		{
			auto tc = std::dynamic_pointer_cast<TestGLRenderableObject>(c);
			assert(tc != nullptr);
			result += prefix + mName + "->" + tc->name() + "\n";
			result += tc->printTree(prefix + "\t");
		}
		return result;
	}
	const std::string &name() const
	{
		return mName;
	}
};

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(TestRenderableObject)
{
	std::weak_ptr<TestGLRenderableObject> o1w, o11w, o12w, o13w, o111w, o112w,
		o113w;
	std::weak_ptr<dream::components::GLMultipleCaleeRenderFunction> o1rfw,
		o11rfw, o12rfw, o13rfw, o111rfw, o112rfw, o113rfw;
	std::shared_ptr<TestGLRenderableObject> o2(
		new TestGLRenderableObject("o2"));

	{
		std::shared_ptr<TestGLRenderableObject> o1(
			new TestGLRenderableObject("o1"));
		o1w	  = o1;
		o1rfw = o1->renderFunction();
		{
			std::shared_ptr<TestGLRenderableObject> o11(
				new TestGLRenderableObject("o11")),
				o12(new TestGLRenderableObject("o12")),
				o13(new TestGLRenderableObject("o13")),
				o111(new TestGLRenderableObject("o111")),
				o112(new TestGLRenderableObject("o112")),
				o113(new TestGLRenderableObject("o113"));
			o11w	= o11;
			o12w	= o12;
			o13w	= o13;
			o111w	= o111;
			o112w	= o112;
			o113w	= o113;
			o11rfw	= o11->renderFunction();
			o12rfw	= o12->renderFunction();
			o13rfw	= o13->renderFunction();
			o111rfw = o111->renderFunction();
			o112rfw = o112->renderFunction();
			o113rfw = o113->renderFunction();

			o1->addChild(o11);
			o1->addChild(o12);
			o1->addChild(o13);

			o11->addChild(o111);
			o11->addChild(o112);
			o11->addChild(o113);

			o2->addChild(o11);

			std::cout << o1->printTree(o1->name() + "tree: ") << std::endl;
		}
		BOOST_TEST(o111w.lock() != nullptr);
		BOOST_TEST(o112w.lock() != nullptr);
		BOOST_TEST(o113w.lock() != nullptr);
		BOOST_TEST(o11w.lock() != nullptr);
		BOOST_TEST(o12w.lock() != nullptr);
		BOOST_TEST(o13w.lock() != nullptr);
		BOOST_TEST(o1w.lock() != nullptr);
		BOOST_TEST(o111rfw.lock() != nullptr);
		BOOST_TEST(o112rfw.lock() != nullptr);
		BOOST_TEST(o113rfw.lock() != nullptr);
		BOOST_TEST(o11rfw.lock() != nullptr);
		BOOST_TEST(o12rfw.lock() != nullptr);
		BOOST_TEST(o13rfw.lock() != nullptr);
		BOOST_TEST(o1rfw.lock() != nullptr);
	}
	BOOST_TEST(o1w.lock() == nullptr);
	BOOST_TEST(o12w.lock() == nullptr);
	BOOST_TEST(o13w.lock() == nullptr);
	BOOST_TEST(o1rfw.lock() == nullptr);
	BOOST_TEST(o12rfw.lock() == nullptr);
	BOOST_TEST(o13rfw.lock() == nullptr);

	BOOST_TEST(o11w.lock() != nullptr);
	BOOST_TEST(o111w.lock() != nullptr);
	BOOST_TEST(o112w.lock() != nullptr);
	BOOST_TEST(o113w.lock() != nullptr);
	BOOST_TEST(o11rfw.lock() != nullptr);
	BOOST_TEST(o111rfw.lock() != nullptr);
	BOOST_TEST(o112rfw.lock() != nullptr);
	BOOST_TEST(o113rfw.lock() != nullptr);
	o2.reset();
	BOOST_TEST(o11w.lock() == nullptr);
	BOOST_TEST(o111w.lock() == nullptr);
	BOOST_TEST(o112w.lock() == nullptr);
	BOOST_TEST(o113w.lock() == nullptr);
	BOOST_TEST(o11rfw.lock() == nullptr);
	BOOST_TEST(o111rfw.lock() == nullptr);
	BOOST_TEST(o112rfw.lock() == nullptr);
	BOOST_TEST(o113rfw.lock() == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
