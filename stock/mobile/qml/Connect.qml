
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

    ColumnLayout {
        spacing: 20
        width: appWindow.width - 40
        id: content

        Text {
            id: msgLabel
            Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            width: parent.width - 10
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 20
            id: row

            TextField {
                id: pwdField
                placeholderText: qsTr( "Password" )
                echoMode: TextInput.Password
                text: connectScreen.pwd
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                implicitHeight: appWindow.minimumCtrlHeight
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
        }

        Button {
            id: connectBtn
            Layout.alignment: Qt.AlignHCenter
            text: qsTr( "Connect" )
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: row.width

            onClicked: {
                showHide.checked = false
                pwdField.echoMode = TextInput.Password
                qmlCppSignals.connectRequest( pwdField.text )
            }
        }
    }
}
