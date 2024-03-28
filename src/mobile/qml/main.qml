
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

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import QtQuick.Window

ApplicationWindow {
    id: appWindow
    visible: true
    width: 400
    height: 600

    property bool connected: false
    property int minimumCtrlHeight: Screen.pixelDensity * 8
    property bool licensesShown: false

    // This is internal property. On true change screen will put product,
    // on false it will take product.
    property bool changeAction

    Menu {
        id: menu
        x: menuButton.x - width + menuButton.width - 2
        y: toolBar.y + toolBar.height + 2

        MenuItem {
            id: changePwdMenu
            text: qsTr( "Reconnect" )
            implicitHeight: minimumCtrlHeight
            enabled: false
            onTriggered: {
                qmlCppSignals.disconnectRequest()
            }
        }

        MenuSeparator {}

        MenuItem {
            id: licenses
            text: qsTr( "Licenses" )
            implicitHeight: minimumCtrlHeight
            onTriggered: {
                stackView.push( licensesView )
                stackView.keyBackEnabled = true
                licensesShown = true
            }
        }
    }

    header: ToolBar {
        id: toolBar
        implicitHeight: minimumCtrlHeight

        RowLayout {
            anchors.fill: parent

            ToolButton {
                id: backBtn
                onClicked: qmlCppSignals.returnBack()
                enabled: stackView.keyBackEnabled
                implicitWidth: parent.height
                implicitHeight: implicitWidth

                Image {
                    id: backBtnImg
                    source: backBtn.enabled ? "qrc:/img/back-enabled_48x48.png" :
                        "qrc:/img/back-disabled_48x48.png"
                    width: parent.width - minimumCtrlHeight / 4
                    height: parent.height - minimumCtrlHeight / 4
                    anchors.centerIn: parent
                }
            }

            Label {
                text: qsTr( "Stock" )
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: menuButton
                onClicked: menu.open()
                enabled: true
                implicitHeight: parent.height
                implicitWidth: implicitHeight

                Image {
                    id: menuBtnImg
                    width: parent.width - minimumCtrlHeight / 4
                    height: parent.height - minimumCtrlHeight / 4
                    anchors.centerIn: parent
                    source: enabled ? "qrc:/img/menu-enabled_48x48.png" :
                        "qrc:/img/menu-disabled_48x48.png"
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
            port: defaultUdpPort
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

    property string unknowmCodeMsg:
		qsTr( "Unknown code. Please contact administrator to add new product to the database. And reconnect after." )

    Component {
        id: changeComponent

        Change {
            id: changeScreen
            put: changeAction

            onUnknownCode: {
				stackView.push( messageComponent )
				stackView.currentItem.message = unknowmCodeMsg
			}
        }
    }

    Component {
        id: messageComponent

        Message {
        }
    }

    Component {
        id: searchComponent

        Search {
			onUnknownCode: {
				stackView.push( messageComponent )
				stackView.currentItem.message = unknowmCodeMsg
			}
        }
    }

    Component {
        id: searchResultComponent

        SearchResult {
            type: connections.searchType
        }
    }

    Component {
		id: cameraViewComponent

		CameraView {
			onCodeChanged: {
				stackView.pop();
				stackView.currentItem.checkCode( code )
			}
		}
    }

    Component {
        id: licensesView

        Licenses {
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        property bool keyBackEnabled: false
        focus: true

        Keys.onReleased: (event) => {
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
            qmlCppSignals.connectRequest( password, defaultUdpPort )
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
        stackView.keyBackEnabled = false

        connected = false
        changePwdMenu.enabled = false
        connectionState.text = qsTr( "Disconnected" )
    }

    Connections {
        id: connections
        target: qmlCppSignals

        property int searchType: 0

        function onConnectionEstablished() {
            stackView.pop()
            stackView.push( actionsComponent )
            stackView.keyBackEnabled = false
            connectionState.text = qsTr( "Connected" )
            changePwdMenu.enabled = true
            connected = true
        }

        function onDisconnected( reason ) {
            disconnected( reason )
        }

        function onConnectRequest( pwd ) {
            connectRequested( pwd )
        }

        function onPutBtnClicked() {
            changeAction = true
            stackView.push( changeComponent )
            stackView.keyBackEnabled = true
        }

        function onTakeBtnClicked() {
            changeAction = false
            stackView.push( changeComponent )
            stackView.keyBackEnabled = true
        }

        function onSearchByCodeBtnClicked() {
            stackView.push( searchComponent )
            stackView.currentItem.model = codesModel
            stackView.currentItem.currentIndex = -1
            stackView.currentItem.byCode = true
            searchType = 0
            stackView.keyBackEnabled = true
        }

        function onSearchByPlaceBtnClicked() {
            stackView.push( searchComponent )
            stackView.currentItem.model = placesModel
            stackView.currentItem.currentIndex = -1
            stackView.currentItem.byCode = false
            searchType = 1
            stackView.keyBackEnabled = true
        }

        function onReturnBack() {
            if( ( stackView.depth > 2 && stackView.keyBackEnabled ) || licensesShown )
                stackView.pop()

            if( licensesShown )
                licensesShown = false

            if( stackView.depth <= 2 )
                stackView.keyBackEnabled = false
        }

        function onPutProduct( code, place, count ) {
            stackView.keyBackEnabled = false
            stackView.push( busyComponent )
        }

        function onTakeProduct( code, place, count ) {
            stackView.keyBackEnabled = false
            stackView.push( busyComponent )
        }

        function onOpOk() {
            stackView.keyBackEnabled = true
            stackView.pop()
            stackView.pop()
            stackView.push( messageComponent )
            stackView.currentItem.message = qsTr( "Data saved. Now you can continue." )
        }

        function onOpFailed() {
            stackView.keyBackEnabled = true
            stackView.pop()
            stackView.push( messageComponent )
            stackView.currentItem.message = qsTr(
                "Something went wrong. Please try again later. Possibly password was changed." )
        }

        function onInternalSearch( data ) {
            stackView.pop();
            stackView.keyBackEnabled = false
            stackView.push( busyComponent )

            if( searchType === 0 )
                qmlCppSignals.search( data, "" )
            else
                qmlCppSignals.search( "", data )
        }

        function onListReceived() {
            stackView.keyBackEnabled = true

            if( searchModel.rowCount() === 0 ) {
                stackView.pop()
                stackView.push( messageComponent )
                stackView.currentItem.message = qsTr( "Nothing found." )
            }
            else {
                stackView.pop()
                stackView.push( searchResultComponent )
            }
        }

        function onShowCamera() {
			stackView.keyBackEnabled = true

			stackView.push( cameraViewComponent )
        }

        function onShowCameraSettings() {
			stackView.keyBackEnabled = true

			stackView.push( stackView.currentItem.cameraSettingsComponentAlias )
        }
    }
}
