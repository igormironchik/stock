
import QtMultimedia 5.15
import QtQuick 2.7
import Frames 0.1

Item {
	id: root
	property string code: "";

	Frames {
		id: frames

		onCodeDetected: {
			root.code = code
		}
	}

	VideoOutput {
		source: frames
		anchors.fill: parent
		anchors.centerIn: parent
		focus : visible
	}
 }
