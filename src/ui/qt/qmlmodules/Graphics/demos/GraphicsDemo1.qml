import QtQuick 2.1
import Graphics 0.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQml 2.1
import QtQuick.Window 2.2

Rectangle {
	color: "red"
	ColumnLayout {
		anchors.fill: parent
		RenderDisplay {
			id: viewport
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.margins: 20
			onViewTransformChanged: sceneView.viewMatrix = view
			SceneView {
				id: sceneView
				property real colorValue: 0
				fov: 45
				aspectRatio: viewport.width/viewport.height
				farPlane: 1000
				nearPlane: 0.1
				MeshObject {
					property vector3d rgb: Qt.vector3d(
						Math.min(1, Math.max(0, -4 * sceneView.colorValue + 2)),
						Math.min(1, Math.max(0, 4 * sceneView.colorValue - 2)),
						(sceneView.colorValue < 0.25)
							? 4 * sceneView.colorValue
							: (sceneView.colorValue > 0.75
								? 4 * (1 - sceneView.colorValue)
								: 1)
					)
					ambient: Qt.rgba(rgb.x, rgb.y, rgb.z, 1)
					mesh: "../assets/Cube.ply"
				}
			}
		}

		RenderDisplay {
			id: viewport2
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.margins: 20
			onViewTransformChanged: sceneView2.viewMatrix = view
			SceneView {
				id: sceneView2
				property real colorValue: 0
				fov: 45
				aspectRatio: viewport.width/viewport.height
				farPlane: 1000
				nearPlane: 0.1

				MeshObject {
					property vector3d rgb: Qt.vector3d(
						Math.min(1, Math.max(0, -4 * sceneView.colorValue + 2)),
						Math.min(1, Math.max(0, 4 * sceneView.colorValue - 2)),
						(sceneView.colorValue < 0.25)
							? 4 * sceneView.colorValue
							: (sceneView.colorValue > 0.75
								? 4 * (1 - sceneView.colorValue)
								: 1)
					)
					ambient: Qt.rgba(rgb.x, rgb.y, rgb.z, 1)
					mesh: "../assets/Cube.ply"
				}
			}
		}
		resources: PropertyAnimation {
			target: sceneView
			property: "colorValue"
			running: true
			loops: Animation.Infinite
			from: 0
			to: 1
			duration: 5000
		}
	}

	Component.onCompleted: popup.open()
}
