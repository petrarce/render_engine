import QtQuick 2.1
import Graphics 0.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQml 2.2
import QtQuick.Window 2.2
import QtQml.Models 2.14

Rectangle {
	color: "red"
	ColumnLayout {
		anchors.fill: parent
		RenderDisplay {
			id: viewport
			objectName: "viewport1"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.margins: 20
			onViewTransformChanged: sceneView.viewMatrix = view
			Component.onCompleted: sceneView.viewMatrix = camera.viewMatrix
			SceneView {
				id: sceneView

				property real colorValue: 0
				fov: 45
				aspectRatio: viewport.width/viewport.height
				farPlane: 1000
				nearPlane: 0.1
				lights: [
					{color: Qt.vector3d(1, 0, 1), position: Qt.vector3d(3, 3, 1)}
				]

				MeshObject {
					id: rootMeshObject
					resources: Instantiator {
						model: 1000
						delegate: MeshObject {
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
							transform: Qt.matrix4x4(1, 0, 0, (index % 20) * 5,
													0, 1, 0, (index / 20) * 5,
													0, 0, 1, 0,
													0, 0, 0, 1)
							renderMode: {
								if(index % 7 === 0)
									return MeshObject.Faces
								if(index % 7 === 1)
									return MeshObject.Lines
								if(index % 7 === 2)
									return MeshObject.Points
								if(index % 7 === 3)
									return MeshObject.FacesLines
								if(index % 7 === 4)
									return MeshObject.FacesPoints
								if(index % 7 === 5)
									return MeshObject.LinesPoints
								if(index % 7 === 6)
									return MeshObject.FacesLinesPoints
							}
						}
						onObjectAdded: {
							rootMeshObject.addRenderableObject(object)
						}
					}
				}
			}
		}

		RenderDisplay {
			id: viewport2
			objectName: "viewport2"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.margins: 20
			onViewTransformChanged: sceneView2.viewMatrix = view
			Component.onCompleted: sceneView2.viewMatrix = camera.viewMatrix
			SceneView {
				id: sceneView2
				property real colorValue: 0
				fov: 45
				aspectRatio: viewport.width/viewport.height
				farPlane: 1000
				nearPlane: 0.1
				lights: [
					{color: Qt.vector3d(1, 1, 1), direction: Qt.vector3d(-1, -2, -3)}
				]

				renderables: [rootMeshObject]
			}
		}
		resources: SequentialAnimation
		{
			running: true
			loops: Animation.Infinite

			PropertyAnimation {
				target: sceneView
				property: "colorValue"
				from: 0
				to: 1
				duration: 2500
			}
			PropertyAnimation {
				target: sceneView
				property: "colorValue"
				from: 1
				to: 0
				duration: 2500
			}

		}
	}
}
