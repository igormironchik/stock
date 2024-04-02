
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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

    signal unknownCode()

    // If this property is true then search will be by product's code,
    // otherwise search will be by place.
    property bool byCode: true

    property alias model: combo.model
    property alias currentIndex: combo.currentIndex

    function checkCode( code ) {
		var i = combo.find( code );

		if( i !== -1 ) {
			combo.currentIndex = i
			okBtn.enabled = true
			stackView.focus = true
		}
		else
			unknownCode();
    }

    Column {
        id: content
        spacing: 20
        width: appWindow.width - 40

        Text {
            id: headerLabel
            anchors.horizontalCenter: content.horizontalCenter
            wrapMode: Text.WordWrap
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: byCode ? qsTr( "Search by Code" ) : qsTr( "Search by Place" )
        }

		Row {
			spacing: 20
			id: comboRow

			ComboBox {
				id: combo
				editable: false
				implicitHeight: appWindow.minimumCtrlHeight
                topInset: 0
                bottomInset: 0
				currentIndex: -1
				textRole: "display"
				implicitWidth: content.width - ( byCode ? scanBtn.implicitWidth + 20 : 0 )
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

			Button {
                id: scanBtn
                width: height
                height: appWindow.minimumCtrlHeight
                implicitHeight: height
                implicitWidth: height
                topInset: 0
                bottomInset: 0
                visible: byCode ? true : false

                Image {
                    width: parent.width - minimumCtrlHeight / 4
                    height: parent.height - minimumCtrlHeight / 4
                    anchors.centerIn: parent
                    source: "qrc:/img/view-barcode_48x48.png"
                }

                onClicked: {
					qmlCppSignals.showCamera()
                }
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
                implicitWidth: ( comboRow.width - 20 ) / 2
                topInset: 0
                bottomInset: 0

                onClicked: {
                    qmlCppSignals.internalSearch( combo.currentText )
                }
            }

            Button {
                id: cancelBtn
                text: qsTr( "Cancel" )
                enabled: true
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( comboRow.width - 20 ) / 2
                topInset: 0
                bottomInset: 0

                onClicked: {
                    qmlCppSignals.returnBack()
                }
            }
        }
    }
}
