#include "GraphicsQmlPlugin.hpp"
#include "RenderDisplay.hpp"
#include <QtQml>

namespace qmlmodule
{
namespace Graphics
{

void GraphicsQmlPlugin::registerTypes(const char *uri)
{
	qmlRegisterType<RenderDisplay>(uri, 0, 1, "RenderDisplay");
}

} // namespace Graphics
} // namespace qmlmodule