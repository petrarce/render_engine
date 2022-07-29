qt_add_qml_module( NAME Graphics
	SOURCES
		GraphicsQmlPlugin.cpp
		GraphicsQmlPlugin.hpp
		RenderDisplay.hpp
		RenderDisplay.cpp
		RenderableObject.hpp
		RenderableObject.cpp
		SceneView.cpp
		SceneView.hpp
		MeshObject.cpp
		MeshObject.hpp
		Camera.hpp
		Camera.cpp
	QMLFILES
		Graphics.qmltypes
	VERSION 0.1
	LINK_LIBS 
		gl-renderer
		geometry-primitives
		qt-helpers
	)

target_deploy_assets(
	TARGET Graphics-qmlmodule
	ASSETFILES
		demos/GraphicsDemo1.qml
		demos/MultiView.qml
		demos/GHS-pictogram-unknown.svg
		demos/MultiView.png
		demos/demo_normalmap.jpg)
