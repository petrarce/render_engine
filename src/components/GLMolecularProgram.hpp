#pragma once
#include <GLWrapperCore>
#include <molecular/gfx/Scope.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include <molecular/util/Scope.h>
#include "Variable.hpp"
namespace dream
{
namespace components
{

template <class BaseClass>
class ScopeLocal : public molecular::util::Scope<BaseClass>
{
public:
	ScopeLocal()
		: molecular::util::Scope<BaseClass>()
	{
	}
	ScopeLocal(const ScopeLocal<BaseClass> &parent)
		: molecular::util::Scope<BaseClass>(parent)
	{
	}
	const std::vector<molecular::util::Hash> &keys() const
	{
		return molecular::util::Scope<BaseClass>::mKeys;
	}
	const std::vector<BaseClass> values() const
	{
		return molecular::util::Scope<BaseClass>::mValues;
	}
};

typedef ScopeLocal<dream::components::Variable> Scope;

class GLMolecularProgram : public glwrapper::GLProgram
{
public:
	GLMolecularProgram()
		: glwrapper::GLProgram()
	{
	}

	void generate(const Scope &scp);
	void prepare(const Scope &scp);

protected:
	size_t mProgramHash = 0;
	std::map<molecular::util::Hash, std::string> mUniforms;
};

} // namespace components
} // namespace dream

namespace molecular
{
namespace util
{
namespace HashUtils
{
inline Hash MakeHash(const dream::components::Scope &scope)
{
	auto &keys = scope.keys();
	Hash id	   = 0;
	for (auto k : keys) id ^= k;
	return id;
}

} // namespace HashUtils
} // namespace util
} // namespace molecular
