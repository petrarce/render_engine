import Graphics 0.1
import QtQuick 2.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

Rectangle {
	RenderDisplay {
		anchors.fill: parent
		id: viewport
		objectName: "viewport1"
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.margins: 20
		onViewTransformChanged: sceneView.viewMatrix = view
		Component.onCompleted: sceneView.viewMatrix = camera.viewMatrix
		SceneView {
			id: sceneView
			fov: 45
			aspectRatio: viewport.width/viewport.height
			farPlane: 1000
			nearPlane: 0.1
			lights: [{color: Qt.vector3d(1, 1, 1), direction: Qt.vector3d(-1, -2, -3)}]
			
			MeshObject {
				mesh: "../assets/Cube.ply"
				property color u_diffuseColor: "white"
			}
			
			MeshObject {
				property string u_map_rgb_uNormalMapTexture: "../assets/demo_normalmap.jpg"
				property color u_diffuseColor: "white"
				mesh: "../assets/Cube.ply"
				transform: Qt.matrix4x4(1, 0, 0, 3,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1)
			}
			
			
			MeshObject {
				property string u_map_r_uAmbientOcclusionMapTexture: "../assets/demo_AOmap.png"
				property color u_diffuseColor: "white"
				mesh: "../assets/Cube.ply"
				transform: Qt.matrix4x4(1, 0, 0, 6,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1)
			}
			MeshObject {
				property string u_map_rgba_uDiffuseTexture: "../assets/demo_diffuseMap.png"
				mesh: "../assets/Cube.ply"
				transform: Qt.matrix4x4(1, 0, 0, 9,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1)
			}
			
			
			MeshObject {
				property string u_map_r_uAmbientOcclusionMapTexture: "../assets/demo_AOmap.png"
				property string u_map_rgb_uNormalMapTexture: "../assets/demo_normalmap.jpg"
				property string u_map_rgba_uDiffuseTexture: "../assets/demo_diffuseMap.png"
				mesh: "../assets/Cube.ply"
				transform: Qt.matrix4x4(1, 0, 0, 12,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1)
			}
		}
	

	}
}