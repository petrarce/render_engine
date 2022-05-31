#pragma once
#include <molecular/util/NonCopyable.h>

namespace dream
{
namespace components
{

template <class TragetClass>
class Singleton : public molecular::util::NonCopyable
{
public:
	static TragetClass &instance()
	{
		static TragetClass sgt;
		return sgt;
	}

protected:
	Singleton(){};
	~Singleton()
	{
	}
};

} // namespace components
} // namespace dream
