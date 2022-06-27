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
				fov: 45
				aspectRatio: viewport.width/viewport.height
				farPlane: 1000
				nearPlane: 0.1
				MeshObject {
					ambient: Qt.rgba(1, 0, 0, 1)
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
		
	}
	resources: Timer {
		interval: 1000
		repeat: true
		running: true
		onTriggered: {
			
			sceneView.fov = sceneView.fov + 1
			console.log("Timer triggered")
		}
	}
}
