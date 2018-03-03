
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

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ScrollView {
    id: screen
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOff
    leftPadding: content.width < width ? ( width - content.width ) / 2 : 0
    topPadding: content.height < height ? ( height - content.height ) / 2 : 0

    // If this property is true then dialog will add new product,
    // otherwise product will be taken.
    property bool put: true

    property alias model: combo.model
    property alias currentIndex: combo.currentIndex

    Component {
        id: comboboxDelegateComponent

        ItemDelegate {
            id: control
            height: appWindow.minimumCtrlHeight
            width: parent.width
            text: model[ "display" ]
            highlighted: parent.highlightedIndex === index
            background.anchors.fill: control
        }
    }

    ColumnLayout {
        id: content
        spacing: 20
        width: appWindow.width - 40

        ComboBox {
            id: combo
            editable: false
            implicitHeight: appWindow.minimumCtrlHeight
            Layout.fillWidth: true
            currentIndex: -1
            textRole: "display"
            delegate: ItemDelegate {
                id: comboDelegateControl
                height: appWindow.minimumCtrlHeight
                width: parent.width
                text: model[ "display" ]
                highlighted: combo.highlightedIndex === index
                background.anchors.fill: comboDelegateControl
            }

            onActivated: {
                focus = false
                okBtn.enabled = true
                stackView.focus = true
            }
        }

        Row {
            spacing: 20
            leftPadding: combo.x

            Button {
                id: okBtn
                text: qsTr( "OK" )
                enabled: false
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( combo.width - 20 ) / 2

                onClicked: {
                    qmlCppSignals.internalSearch( combo.currentText )
                }
            }

            Button {
                id: cancelBtn
                text: qsTr( "Cancel" )
                enabled: true
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( combo.width - 20 ) / 2

                onClicked: {
                    qmlCppSignals.returnBack()
                }
            }
        }
    }
}