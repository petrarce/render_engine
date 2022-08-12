import Graphics 0.1
import QtQml 2.12
import QtQuick 2.12
Rectangle {
	RenderDisplay {
		id: viewport
		defaultCamera.angleX: 45
		defaultCamera.angleZ: 45
		anchors.fill: parent
		onViewTransformChanged: sceneView.viewMatrix = view
		Component.onCompleted: sceneView.viewMatrix = camera.viewMatrix
		SceneView {
			id: sceneView

			lights: [
				{
					color: Qt.vector3d(1, 1, 1),
					position: Qt.vector3d(0, 0, 0.1),
					attenuationDistance: 20
//					direction: Qt.vector3d(-1, -2, -3)
				}
			]
			MeshObject {
				objectName: "Custom triangle"
				property color u_diffuseColor: Qt.rgba(1, 0, 0, 1)
				mesh: {
					"vertices": [
						Qt.vector3d(-1, 1, 0),
						Qt.vector3d(-1, -1, 0),
						Qt.vector3d(1, 1, 0),
						Qt.vector3d(1, -1, 0),
					],
					"normals": [
						Qt.vector3d(0, 0, 1),
						Qt.vector3d(0, 0, 1),
						Qt.vector3d(0, 0, 1),
						Qt.vector3d(0, 0, 1)
					],
					"indices": [
						0, 1, 2,
						2, 1, 3,
					]
				}
				transform: Qt.matrix4x4(1, 0, 0, 0,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1)
			}
		}
	}

}
