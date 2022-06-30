#include "GraphicsQmlPlugin.hpp"
#include "RenderDisplay.hpp"
#include "SceneView.hpp"
#include "RenderableObject.hpp"
#include "MeshObject.hpp"
#include "Camera.hpp"
#include <QtQml>

using namespace qmlmodule::Graphics;

void GraphicsQmlPlugin::registerTypes(const char *uri)
{
	qmlRegisterType<RenderDisplay>(uri, 0, 1, "RenderDisplay");
	qmlRegisterType<SceneView>(uri, 0, 1, "SceneView");
	qmlRegisterType<MeshObject>(uri, 0, 1, "MeshObject");
	qmlRegisterType<OrbitCamera>(uri, 0, 1, "OrbitCamera");

	qmlRegisterAnonymousType<RenderableObject>(uri, 0);
	qmlRegisterAnonymousType<Camera>(uri, 0);
}
