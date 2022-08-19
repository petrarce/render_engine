#pragma once
#include "MeshObject.hpp"
#include <src/components/Singleton.hpp>
namespace dream
{
namespace geometry
{

template <class Constructable>
class GLBuilder : public components::Singleton<GLBuilder<Constructable>>
{
private:
	virtual Constructable *construct() = 0;
};

template <class Constructable>
class GLFactory : public components::Singleton<GLFactory<Constructable>>
{
	template <class T,
			  std::enable_if_t<
				  std::is_base_of<GLBuilder<Constructable>, T>::value, bool>>
	static Constructable *construct(const T &builder)
	{
		return builder.construct();
	}

	template <class T,
			  std::enable_if_t<
				  std::is_base_of<GLBuilder<Constructable>, T>::value, bool>>
	static Constructable *construct()
	{
		auto &builder = typename std::enable_if<
			std::is_base_of<GLBuilder<Constructable>, T>::value,
			T>::type::instance();
		return builder.construct();
	}
};

} // namespace geometry
} // namespace dream
