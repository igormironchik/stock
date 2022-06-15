
import QtMultimedia
import QtQuick 2.7
import Frames 0.1
import QtQuick.Controls 2.2

Item {
	id: root
	property string code: "";
	property Component cameraSettingsComponentAlias: cameraSettingsComponent

	Component {
		id: cameraSettingsComponent

		CameraSettings {
			image: frames.currentFrame
			Component.onDestruction: cameraSettings.applySettings()
		}
    }

	Frames {
        id: frames
        videoSink: view.videoSink

        onCodeDetected: (code) => {
			root.code = code
		}
	}

    VideoOutput {
        id: view
        anchors.fill: parent
        anchors.centerIn: parent
        focus : visible
        transform: [ Scale { origin.x: root.width / 2; origin.y: root.height / 2;
                xScale: frames.xScale; yScale: frames.yScale },
            Rotation { origin.x: root.width / 2; origin.y: root.height / 2;
                angle: frames.angle } ]
    }

	Button {
		id: settings
		width: height
		height: appWindow.minimumCtrlHeight
		implicitHeight: height
		implicitWidth: height
		x: parent.x + parent.width - width - 20
		y:  parent.y + parent.height - height - 20

		Image {
            width: parent.width - minimumCtrlHeight / 4
            height: parent.height - minimumCtrlHeight / 4
            anchors.centerIn: parent
			source: "qrc:/img/settings_48x48.png"
		}

		onClicked: {
			qmlCppSignals.showCameraSettings();
		}
	}
 }
