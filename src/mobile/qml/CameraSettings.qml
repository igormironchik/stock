/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

import QtQuick 2.15
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import PreviewImage 0.1

ScrollView {
	ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
	ScrollBar.vertical.policy: ScrollBar.AlwaysOff
	leftPadding: content.width < width ? ( width - content.width ) / 2 : 0
	topPadding: content.height < height ? ( height - content.height ) / 2 : 0
	id: camSettingsScreen

	property alias image: preview.image

	Column {
		spacing: 20
		width: appWindow.width - 40
		id: content

		ComboBox {
			id: camera
			model: cameraModel
			editable: false
			implicitHeight: appWindow.minimumCtrlHeight
			implicitWidth: content.width
            topInset: 0
            bottomInset: 0
			currentIndex: -1
			textRole: "display"
			delegate: ItemDelegate {
				id: cameraDelegateControl
				height: appWindow.minimumCtrlHeight
				width: parent.width
				text: model[ "display" ]
				highlighted: camera.highlightedIndex === index
				background.anchors.fill: cameraDelegateControl
			}

			onActivated: {
				cameraSettings.setCamName( currentText, false )
			}
		}

		ComboBox {
			id: resolution
			model: resolutionModel
			editable: false
			implicitHeight: appWindow.minimumCtrlHeight
			implicitWidth: content.width
            topInset: 0
            bottomInset: 0
			currentIndex: -1
			textRole: "display"
			delegate: ItemDelegate {
				id: resolutionDelegateControl
				height: appWindow.minimumCtrlHeight
				width: parent.width
				text: model[ "display" ]
				highlighted: resolution.highlightedIndex === index
				background.anchors.fill: resolutionDelegateControl
			}

			onActivated: {
				cameraSettings.setCamSettings( currentText, false )
			}
		}

		PreviewImage {
			id: preview
			width: content.width / 1.5
			height: width
			anchors.horizontalCenter: parent.horizontalCenter
		}

		Row {
			anchors.right: parent.right
			spacing: 20

			Button {
                id: mirror
                width: height
                height: appWindow.minimumCtrlHeight
                implicitHeight: height
                implicitWidth: height
                topInset: 0
                bottomInset: 0

                Image {
                    width: parent.width - minimumCtrlHeight / 4
                    height: parent.height - minimumCtrlHeight / 4
                    anchors.centerIn: parent
                    source: "qrc:/img/mirror_48x48.png"
                }

                onClicked: {
					cameraSettings.mirror()
                }
            }

            Button {
                id: rotate
                width: height
                height: appWindow.minimumCtrlHeight
                implicitHeight: height
                implicitWidth: height
                topInset: 0
                bottomInset: 0

                Image {
                    width: parent.width - minimumCtrlHeight / 4
                    height: parent.height - minimumCtrlHeight / 4
                    anchors.centerIn: parent
                    source: "qrc:/img/rotate_48x48.png"
                }

                onClicked: {
					cameraSettings.rotate()
                }
            }
		}
	}

	Component.onCompleted: {
		camera.currentIndex = camera.find( cameraSettings.currentCamName )
		resolution.currentIndex = resolution.find( cameraSettings.currentResolution )
	}
}
