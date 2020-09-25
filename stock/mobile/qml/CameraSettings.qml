/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

                Image {
                    anchors.fill: parent
                    source: "qrc:/img/mirror_48x48.png"
                }

                onClicked: {
                }
            }

            Button {
                id: rotate
                width: height
                height: appWindow.minimumCtrlHeight
                implicitHeight: height
                implicitWidth: height

                Image {
                    anchors.fill: parent
                    source: "qrc:/img/rotate_48x48.png"
                }

                onClicked: {
                }
            }
		}
	}

	Component.onCompleted: {
		camera.currentIndex = camera.find( cameraSettings.currentCamName )
		resolution.currentIndex = resolution.find( cameraSettings.currentResolution )
	}
}
