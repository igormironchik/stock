
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

import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick 2.7

ApplicationWindow {
    id: appWindow
    visible: true
    width: 400
    height: 600

    property bool connected: false

    Menu {
        id: menu
        x: menuButton.x - width + menuButton.width - 2
        y: toolBar.y + toolBar.height

        MenuItem {
            text: qsTr( "Change Password" )
            onTriggered: {
                qmlCppSignals.disconnectRequest()
            }
        }
    }

    header: ToolBar {
        id: toolBar

        RowLayout {
            anchors.fill: parent

            Label {
                text: qsTr( "Stock" )
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: menuButton
                onClicked: menu.open()
                enabled: false

                Image {
                    id: menuBtnImg
                    anchors.centerIn: parent
                    width: parent.width - 20
                    height: width
                    source: "qrc:/img/menu-disabled_48x48.png"
                }

                onEnabledChanged: {
                    if( enabled )
                        menuBtnImg.source = "qrc:/img/menu-enabled_48x48.png"
                    else
                        menuBtnImg.source = "qrc:/img/menu-disabled_48x48.png"
                }
            }
        }
    }

    footer: Label {
        id: connectionState
        text: qsTr( "Disconnected" )
    }

    Component {
        id: connectComponent

        Connect {
            pwd: password
        }
    }

    Component {
        id: actionsComponent

        Actions {
        }
    }

    Component {
        id: busyComponent

        Busy {
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        property bool keyBackEnabled: true
        focus: true

        Keys.onReleased: {
            if( event.key === Qt.Key_Back && stackView.depth > 2 &&
                 keyBackEnabled ) {
                     stackView.pop();
                     event.accepted = true;
             }
        }

        initialItem: connectComponent
    }

    Component.onCompleted: {
        if( passwordSet ) {
            qmlCppSignals.connectRequest( password )
        }
    }

    function connectRequested( pwd ) {
        password = pwd
        stackView.keyBackEnabled = false
        stackView.push( busyComponent )
        connectionState.text = qsTr( "Disconnected" )
    }

    function disconnected( msg ) {
        while( stackView.depth > 1 )
            stackView.pop();

        stackView.currentItem.message = msg

        connected = false
        menuButton.enabled = false
        connectionState.text = qsTr( "Disconnected" )
    }

    Connections {
        target: qmlCppSignals

        onConnectionEstablished: {
            stackView.pop()
            stackView.push( actionsComponent )
            stackView.keyBackEnabled = true
            connectionState.text = qsTr( "Connected" )
            menuButton.enabled = true
            connected = true
        }

        onDisconnected: {
            disconnected( reason )
        }

        onConnectRequest: {
            connectRequested( pwd )
        }
    }
}
