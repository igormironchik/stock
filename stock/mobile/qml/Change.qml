
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

    ColumnLayout {
        id: content
        spacing: 20
        width: appWindow.width - 40

        Label {
            height: appWindow.minimumCtrlHeight
            text: ( put ? qsTr( "Put Product" ) : qsTr( "Take Product" ) )
            Layout.alignment: Qt.AlignHCenter
        }

        GridLayout {
            columnSpacing: 20
            rowSpacing: 20
            rows: 3
            flow: GridLayout.TopToBottom
            width: parent.width

            Label {
                text: qsTr( "Code" )
            }

            Label {
                text: qsTr( "Place" )
            }

            Label {
                text: qsTr( "Amount" )
            }

            ComboBox {
                id: code
                model: codesModel
                editable: false
                implicitHeight: appWindow.minimumCtrlHeight
                Layout.fillWidth: true
                currentIndex: -1
                textRole: "display"
                delegate: ItemDelegate {
                    id: codeDelegateControl
                    height: appWindow.minimumCtrlHeight
                    width: parent.width
                    text: model[ "display" ]
                    highlighted: code.highlightedIndex === index
                    background.anchors.fill: codeDelegateControl
                }

                onActivated: {
                    check()
                }
            }

            ComboBox {
                id: place
                model: placesModel
                editable: false
                implicitHeight: appWindow.minimumCtrlHeight
                Layout.fillWidth: true
                currentIndex: -1
                textRole: "display"
                delegate: ItemDelegate {
                    id: placeDelegateControl
                    height: appWindow.minimumCtrlHeight
                    width: parent.width
                    text: model[ "display" ]
                    highlighted: place.highlightedIndex === index
                    background.anchors.fill: placeDelegateControl
                }

                onActivated: {
                    check()
                }
            }

            SpinBox {
                id: amount
                from: 0
                value: 0
                editable: false
                implicitHeight: appWindow.minimumCtrlHeight
                Layout.fillWidth: true
                up.indicator.implicitWidth: appWindow.minimumCtrlHeight
                down.indicator.implicitWidth: appWindow.minimumCtrlHeight

                onValueChanged: {
                    check()
                }
            }

            Button {
                id: scanBtn
                width: height
                height: appWindow.minimumCtrlHeight
                implicitHeight: height
                implicitWidth: height

                Image {
                    anchors.fill: parent
                    source: "qrc:/img/view-barcode_48x48.png"
                }
            }
        }

        Row {
            spacing: 20
            leftPadding: place.x

            Button {
                id: okBtn
                text: qsTr( "OK" )
                enabled: false
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( place.width - 20 ) / 2

                onClicked: {
                    if( put )
                        qmlCppSignals.putProduct( code.currentText,
                            place.currentText, amount.value )
                    else
                        qmlCppSignals.takeProduct( code.currentText,
                            place.currentText, amount.value )
                }
            }

            Button {
                id: cancelBtn
                text: qsTr( "Cancel" )
                enabled: true
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( place.width - 20 ) / 2

                onClicked: {
                    qmlCppSignals.returnBack()
                }
            }
        }
    }

    function check() {
        if( code.currentText.length != 0 &&
            place.currentText.length != 0 &&
            amount.value > 0 )
                okBtn.enabled = true
        else
            okBtn.enabled = false

        code.focus = false
        place.focus = false
        amount.focus = false
        stackView.focus = true
    }
}
