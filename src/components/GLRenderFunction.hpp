#pragma once
#include "GLMolecularProgram.hpp"
#include <variant>

namespace std
{

template <class T>
struct type
{
};
template <class T>
constexpr type<T> type_v;

template <class T, class... Ts, template <class...> class Tp>
constexpr bool is_one_of(type<Tp<Ts...>>, type<T>)
{
	return (std::is_same_v<Ts, T> || ...);
}

} // namespace std

namespace dream
{
namespace components
{

class GLRenderFunction
{
public:
	using UniformVariant_t =
		std::variant<int, float, Eigen::Vector2f, Eigen::Vector3f,
					 Eigen::Vector4f, Eigen::Matrix3f, Eigen::Matrix4f>;
	void draw(const Scope &parentScope = Scope())
	{
		assert(mInitialized && "Render function was not initialized");
		drawImpl(parentScope);
	}

	virtual void init()
	{
		mInitialized = true;
	}

	bool isInitialized()
	{
		return mInitialized;
	}

	template <class T,
			  typename std::enable_if_t<
				  std::is_one_of(std::type_v<UniformVariant_t>, std::type_v<T>),
				  bool> = true>
	void setUniform(const std::string &name, const T &val)
	{
		mUniforms.insert({ molecular::util::HashUtils::MakeHash(name),
						   UniformVariant_t(val) });
	}

	template <
		class T,
		typename std::enable_if_t<!std::is_one_of(std::type_v<UniformVariant_t>,
												  std::type_v<T>),
								  bool> = true,
		bool Enable = std::is_one_of(std::type_v<UniformVariant_t>,
									 std::type_v<T>)>
	void setUniform(const std::string &name, const T &val)
	{
		static_assert(Enable,
					  "Add required type (see error stack for missing type) to "
					  "the definition of the UniformVariant_t");
	}

	void unsetUniform(const std::string &name)
	{
		auto hash = molecular::util::HashUtils::MakeHash(name);
		auto it	  = mUniforms.find(hash);
		if (it != mUniforms.end())
			mUniforms.erase(it);
	}

protected:
	virtual void drawImpl(const Scope &parentScope) = 0;
	virtual void prepareScope(Scope &scope)
	{
		for (const auto &u : mUniforms)
		{
			std::visit(
				[&u, &scope](auto &&val)
				{
					using T = std::decay_t<decltype(val)>;
					scope.Set(u.first, Uniform<T>(val));
				},
				u.second);
		}
	}

private:
	std::map<molecular::util::Hash, UniformVariant_t> mUniforms;
	bool mInitialized{ false };
};

class GLSingleCaleeRenderFunction : public GLRenderFunction
{
public:
	GLSingleCaleeRenderFunction(
		std::shared_ptr<GLRenderFunction> calee = nullptr);
	void setCalee(std::shared_ptr<GLRenderFunction> calee);

protected:
	void drawImpl(const Scope &parentScope) override;

	std::shared_ptr<GLRenderFunction> mCalee;
};

class GLMultipleCaleeRenderFunction : public GLRenderFunction
{
public:
	using CaleePtr	 = std::shared_ptr<GLRenderFunction>;
	using CaleeArray = std::vector<CaleePtr>;
	GLMultipleCaleeRenderFunction(const CaleeArray &calees = {});
	virtual ~GLMultipleCaleeRenderFunction()
	{
	}

	void setCalees(const CaleeArray &calees);
	void addCalee(CaleePtr calee)
	{
		mCalees.push_back(calee);
	}
	void removeCalee(CaleePtr calee)
	{
		std::remove_if(mCalees.begin(), mCalees.end(),
					   [calee](CaleePtr item) { return item == calee; });
	}

	void init() override
	{
		GLRenderFunction::init();
	}

protected:
	void drawImpl(const Scope &parentScope) override;

	CaleeArray mCalees;
};
} // namespace components
} // namespace dream
