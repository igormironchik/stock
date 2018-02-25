
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

import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: connectScreen

    property string pwd
    property alias message: msgLabel.text

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            id: msgLabel
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WordWrap
            width: parent.width - 10
            horizontalAlignment: Text.AlignHCenter
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            TextField {
                id: pwdField
                placeholderText: qsTr( "Password" )
                echoMode: TextInput.Password
                text: connectScreen.pwd
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
            }

            Button {
                id: showHide
                checkable: true
                checked: false
                width: height
                height: parent.height
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
        }

        Button {
            id: connectBtn
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr( "Connect" )

            onClicked: {
                showHide.checked = false
                pwdField.echoMode = TextInput.Password
                qmlCppSignals.connectRequest( pwdField.text )
            }
        }
    }
}
