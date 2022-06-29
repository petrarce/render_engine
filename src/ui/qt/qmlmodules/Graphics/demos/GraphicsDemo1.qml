import QtQuick 2.1
import Graphics 0.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQml 2.1

Rectangle {
	color: "red"
	ColumnLayout {
		anchors.fill: parent
		RenderDisplay {
			id: viewport
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.margins: 20
			SceneView {
				id: sceneView
				property real colorValue: 0
				fov: 45
				aspectRatio: viewport.width/viewport.height
				farPlane: 1000
				nearPlane: 0.1
				MeshObject {
					ambient: Qt.rgba(	Math.min(1, Math.max(0, 4 * sceneView.colorValue - 2)), 
										Math.min(1, Math.max(0, -4 * sceneView.colorValue + 2)), 
										(sceneView.colorValue < 0.25) 
										? sceneView.colorValue 
										: sceneView.colorValue > 0.75
											? 3 - 3 * sceneView.colorValue
											: 1,
										1)
//					ambient: Qt.rgba(0.5, 0.5, 1, 1)
					onAmbientChanged: console.log(ambient)
					mesh: "../assets/Cube.ply"
				}
			}
		}
		Rectangle{
			Layout.fillWidth: true
			Layout.fillHeight: true
			color: "green"
			Button {
				anchors.fill: parent
				Layout.fillWidth: true
				Layout.fillHeight: true
				text: "Update"
				onClicked: mesh.update()
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
}
