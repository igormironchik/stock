
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
                leftPadding: appWindow.minimumCtrlHeight / 3
                echoMode: TextInput.Password
                text: connectScreen.pwd
                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                height: Math.max( appWindow.minimumCtrlHeight, implicitHeight )
                width: content.width - 40 - appWindow.minimumCtrlHeight - pwdText.width
                verticalAlignment: TextInput.AlignVCenter
            }

            Button {
                id: showHide
                checkable: true
                checked: false
                width: height
                height: implicitHeight
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: implicitHeight
                topInset: 0
                bottomInset: 0

                Image {
                    id: img
                    width: parent.width - minimumCtrlHeight / 4
                    height: parent.height - minimumCtrlHeight / 4
                    anchors.centerIn: parent
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
                topInset: 0
                bottomInset: 0
                enabled: pwdField.text.length != 0

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
