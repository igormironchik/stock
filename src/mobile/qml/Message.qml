
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

    property alias message: msgLabel.text

    Column {
        id: content
        spacing: 20
        width: appWindow.width - 40

        Text {
            id: msgLabel
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WordWrap
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: okBtn
            text: qsTr( "OK" )
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: appWindow.minimumCtrlHeight * 3
            topInset: 0
            bottomInset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                qmlCppSignals.returnBack()
            }
        }
    }
}

