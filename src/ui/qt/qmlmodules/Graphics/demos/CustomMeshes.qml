import Graphics 0.1
import QtQml 2.12
import QtQuick 2.12
Rectangle {
	RenderDisplay {
		id: viewport
		defaultCamera.angleX: -45
		defaultCamera.angleZ: 45
		defaultCamera.distance: 2
		anchors.fill: parent
		onViewTransformChanged: sceneView.viewMatrix = view
		Component.onCompleted: sceneView.viewMatrix = camera.viewMatrix
		SceneView {
			id: sceneView

			lights: [
				{
					color: Qt.vector3d(1, 1, 1),
					position: Qt.vector3d(0, 0, 1),
					attenuationDistance: 5
//					direction: Qt.vector3d(-1, -2, -3)
				}
			]
			MeshObject {
//				property real u_ambientTerm: 1
//				property real u_diffuseTerm: 0
//				property real u_specularTerm: 0
				objectName: "Custom triangle"
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
					"colors": [
						Qt.vector4d(0, 0, 0, 1),
						Qt.vector4d(0, 0, 0, 1),
						Qt.vector4d(1, 1, 1, 1),
						Qt.vector4d(1, 1, 1, 1),
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
