
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
    ScrollBar.vertical.policy: ScrollBar.AsNeeded

    //! 0 - by product search, 1 - by place.
    property int type: 0

    Column {
        width: appWindow.width
        id: content

        Repeater {
            model: searchModel

            delegate: Item {
                implicitWidth: content.width
                implicitHeight: row.implicitHeight + 11

                Rectangle {
                    color: "#C7C8C9FF"
                    anchors.bottom: parent.bottom
                    height: 1
                    width: parent.width - 5
                    x: 5
                }

                Row {
                    id: row
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 20
                    width: parent.width

                    Column {
                        id: col
                        spacing: 5

                        Text {
                            font.pixelSize: appWindow.minimumCtrlHeight / 2.5
                            text: type === 0 ? qsTr( "<b>Place: </b>" ) + model[ "PlaceRole" ] :
                                qsTr( "<b>Code: </b>" ) + model[ "CodeRole" ]
                        }

                        Text {
                            font.pixelSize: appWindow.minimumCtrlHeight / 2.5
                            text: qsTr( "<b>Amount: </b>" ) + model[ "AmountRole" ]
                        }
                    }

                    Column {
                        width: parent.width - col.width - 20
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
							font.pixelSize: appWindow.minimumCtrlHeight / 3
                            width: parent.width
                            text: model[ "DescRole" ]
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }
    }
}
