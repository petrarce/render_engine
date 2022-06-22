#include "GraphicsQmlPlugin.hpp"
#include "RenderDisplay.hpp"
#include "SceneView.hpp"
#include "RenderableObject.hpp"
#include <QtQml>

namespace qmlmodule
{
namespace Graphics
{

void GraphicsQmlPlugin::registerTypes(const char *uri)
{
	qmlRegisterType<RenderDisplay>(uri, 0, 1, "RenderDisplay");
	qmlRegisterType<SceneView>(uri, 0, 1, "SceneView");

	qmlRegisterUncreatableType<RenderableObject>(
		uri, 0, 1, "RenderableObject",
		"RenderableObject is just for refference");
}

} // namespace Graphics
} // namespace qmlmodule