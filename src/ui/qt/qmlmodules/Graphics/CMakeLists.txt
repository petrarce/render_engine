qt_add_qml_module( NAME Graphics
	SOURCES
		GraphicsQmlPlugin.cpp
		GraphicsQmlPlugin.hpp
		RenderDisplay.hpp
		RenderDisplay.cpp
	QMLFILES
		Graphics.qmltypes
	VERSION 0.1
	)

target_deploy_assets(
	TARGET Graphics-qmlmodule
	ASSETFILES
		demos/GraphicsDemo1.qml)
