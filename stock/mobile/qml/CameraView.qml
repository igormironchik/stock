
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
			anchors.fill: parent
			source: "qrc:/img/settings_48x48.png"
		}

		onClicked: {
			qmlCppSignals.showCameraSettings();
		}
	}
 }
