import QtQuick 2.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQml 2.2
import QtQuick.Window 2.2
import QtQml.Models 2.14

Rectangle {
	color: "transparent"

	RowLayout {
		anchors.fill: parent
		ListView {
			id: modeSelect
			property string selectedItem: "Multi View"
			
			Layout.preferredWidth: 200
			Layout.fillHeight: true
			
			model: ListModel {
				dynamicRoles: true
				Component.onCompleted: {
					append({name: "Multi View", preview: "MultiView.png"})
					append({name: "Custom Meshes"})
					append({name: "Terrain View"})
				}
			}

			delegate: Rectangle {
				width: ListView.view.width
				height: 200
				color: model.name === modeSelect.selectedItem
					? "lightblue"
					: "transparent"
				MouseArea {
					anchors.fill: parent
					onPressed: {
						modeSelect.selectedItem = model.name
					}
				}
				ColumnLayout {
					anchors.fill: parent
					anchors.margins: 5
					Image {
						Layout.fillWidth: true
						Layout.fillHeight: true
						source: model.preview !== undefined  
								? Qt.resolvedUrl(model.preview)
								: "GHS-pictogram-unknown.svg"
					}
					Text {
						Layout.minimumHeight: 12
						Layout.preferredHeight: 20
						Layout.fillWidth: true
						text: model.name
						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
						font.pointSize: 12
						
					}
					
				}
			}
		}
		
		MultiView {
			visible: modeSelect.selectedItem === "Multi View"
			Layout.fillWidth: true
			Layout.fillHeight: true
		}

		CustomMeshes {
			visible: modeSelect.selectedItem === "Custom Meshes"
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
	}
}
