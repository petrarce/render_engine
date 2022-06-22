import QtQuick 2.1
import Graphics 0.1

Rectangle {
	color: "red"
	RenderDisplay {
		anchors.fill: parent
		anchors.margins: 20
		Rectangle {
			anchors.fill:parent
			color: "green"
		}
	}
}
