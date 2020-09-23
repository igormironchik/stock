
import QtMultimedia 5.15
import QtQuick 2.7
import Frames 0.1

Item {
	Frames {
		id: frames
	}

	VideoOutput {
		source: frames
		anchors.fill: parent
		focus : visible
	}
 }
