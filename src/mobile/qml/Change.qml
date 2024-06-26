
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

import QtQuick 2.15
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ScrollView {
    id: screen
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOff
    leftPadding: content.width < width ? ( width - content.width ) / 2 : 0
    topPadding: content.height < height ? ( height - content.height ) / 2 : 0

    signal unknownCode()

    // If this property is true then dialog will add new product,
    // otherwise product will be taken.
    property bool put: true

    function checkCode( code ) {
		var i = codeField.find( code );

		if( i !== -1 ) {
			codeField.currentIndex = i
			check();
		}
		else
			unknownCode();
    }

    Column {
        id: content
        spacing: 20
        width: appWindow.width - 40

        Label {
            height: appWindow.minimumCtrlHeight
            text: ( put ? qsTr( "Put Product" ) : qsTr( "Take Product" ) )
            anchors.horizontalCenter: content.horizontalCenter
        }

        Grid {
            columnSpacing: 20
            rowSpacing: 20
            rows: 3
            flow: Grid.TopToBottom
            width: parent.width
            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignLeft

            Label {
                text: qsTr( "Code" )
                id: codeLabel
            }

            Label {
                text: qsTr( "Place" )
                id: placeLabel
            }

            Label {
                text: qsTr( "Amount" )
                id: amountLabel
            }

            ComboBox {
                id: codeField
                model: codesModel
                editable: false
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: content.width -
					Math.max( codeLabel.width, placeLabel.width, amountLabel.width ) -
					scanBtn.implicitWidth - 40
                topInset: 0
                bottomInset: 0
                currentIndex: -1
                textRole: "display"
                delegate: ItemDelegate {
                    id: codeDelegateControl
                    height: appWindow.minimumCtrlHeight
                    width: parent.width
                    text: model[ "display" ]
                    highlighted: codeField.highlightedIndex === index
                    background.anchors.fill: codeDelegateControl
                }

                onActivated: {
                    check()
                }
            }

            ComboBox {
                id: placeField
                model: placesModel
                editable: false
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: codeField.implicitWidth
                topInset: 0
                bottomInset: 0
                currentIndex: -1
                textRole: "display"
                delegate: ItemDelegate {
                    id: placeDelegateControl
                    height: appWindow.minimumCtrlHeight
                    width: parent.width
                    text: model[ "display" ]
                    highlighted: placeField.highlightedIndex === index
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
                implicitWidth: codeField.implicitWidth
                //up.indicator.implicitWidth: appWindow.minimumCtrlHeight
                //down.indicator.implicitWidth: appWindow.minimumCtrlHeight

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
                topInset: 0
                bottomInset: 0

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
            leftPadding: placeField.x

            Button {
                id: okBtn
                text: qsTr( "OK" )
                enabled: false
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( placeField.width - 20 ) / 2
                topInset: 0
                bottomInset: 0

                onClicked: {
                    if( put )
                        qmlCppSignals.putProduct( codeField.currentText,
                            placeField.currentText, amount.value )
                    else
                        qmlCppSignals.takeProduct( codeField.currentText,
                            placeField.currentText, amount.value )
                }
            }

            Button {
                id: cancelBtn
                text: qsTr( "Cancel" )
                enabled: true
                implicitHeight: appWindow.minimumCtrlHeight
                implicitWidth: ( placeField.width - 20 ) / 2
                topInset: 0
                bottomInset: 0

                onClicked: {
                    qmlCppSignals.returnBack()
                }
            }
        }
    }

    function check() {
        if( codeField.currentText.length != 0 &&
            placeField.currentText.length != 0 &&
            amount.value > 0 )
                okBtn.enabled = true
        else
            okBtn.enabled = false

        codeField.focus = false
        placeField.focus = false
        amount.focus = false
        stackView.focus = true
    }
}
