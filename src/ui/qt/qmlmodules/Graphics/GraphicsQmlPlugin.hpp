#pragma once
#include <QQmlExtensionPlugin>
#include <QQmlExtensionInterface>
namespace qmlmodule
{
namespace Graphics
{

class GraphicsQmlPlugin : public QQmlExtensionPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
	void registerTypes(const char *uri) override;
};
} // namespace Graphics
} // namespace qmlmodule