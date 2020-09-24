
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

ScrollView {
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOff
    leftPadding: content.width < width ? ( width - content.width ) / 2 : 0
    topPadding: content.height < height ? ( height - content.height ) / 2 : 0
    id: connectScreen

    property string pwd
    property alias message: msgLabel.text
    property int port

    Column {
        spacing: 20
        width: appWindow.width - 40
        id: content

        Text {
            id: msgLabel
            anchors.horizontalCenter: content.horizontalCenter
            wrapMode: Text.WordWrap
            width: parent.width - 10
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Grid {
            columns: 3
            rowSpacing: 20
            columnSpacing: 20
            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignLeft

            Text {
                text: qsTr( "Port" )
            }

            SpinBox {
                id: portField
                editable: true
                from: 1
                to: 65535
                value: connectScreen.port
                textFromValue: function( value ) {
                   return value;
                }
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: pwdField.width
            }

            Rectangle {
                width: 1
                height: 1
            }

            Text {
                text: qsTr( "Password" )
                id: pwdText
            }

            TextField {
                id: pwdField
                placeholderText: qsTr( "Password" )
                echoMode: TextInput.Password
                text: connectScreen.pwd
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: content.width - 40 - implicitHeight - pwdText.width
            }

            Button {
                id: showHide
                checkable: true
                checked: false
                width: height
                height: appWindow.minimumCtrlHeight
                implicitHeight: height
                implicitWidth: height

                Image {
                    id: img
                    anchors.fill: parent
                    source: "qrc:/img/layer-visible-on_48x48.png"
                }

                onClicked: {
                    if( checked ) {
                        img.source = "qrc:/img/layer-visible-off_48x48.png"
                        pwdField.echoMode = TextInput.Normal
                    } else {
                        img.source = "qrc:/img/layer-visible-on_48x48.png"
                        pwdField.echoMode = TextInput.Password
                    }
                }
            }

            Rectangle {
                width: 1
                height: 1
            }

            Button {
                id: connectBtn
                text: qsTr( "Connect" )
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: pwdField.width

                onClicked: {
                    showHide.checked = false
                    pwdField.echoMode = TextInput.Password
                    qmlCppSignals.connectRequest( pwdField.text, portField.value )
                }
            }

            Rectangle {
                width: 1
                height: 1
            }
        }
    }
}
