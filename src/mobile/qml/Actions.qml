
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ScrollView {
    id: screen
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOff
    leftPadding: content.width < width ? ( width - content.width ) / 2 : 0
    topPadding: content.height < height ? ( height - content.height ) / 2 : 0

    Column {
        id: content
        spacing: 20
        width: appWindow.width - 40

        Button {
            id: put
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            topInset: 0
            bottomInset: 0
            text: qsTr( "Put Product" )

            onClicked: {
                qmlCppSignals.putBtnClicked()
            }
        }

        Button {
            id: take
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            topInset: 0
            bottomInset: 0
            text: qsTr( "Take Product" )

            onClicked: {
                qmlCppSignals.takeBtnClicked()
            }
        }

        Button {
            id: searchByCode
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            topInset: 0
            bottomInset: 0
            text: qsTr( "Search By Code" )

            onClicked: {
                qmlCppSignals.searchByCodeBtnClicked()
            }
        }

        Button {
            id: searchByPlace
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            topInset: 0
            bottomInset: 0
            text: qsTr( "Search By Place" )

            onClicked: {
                qmlCppSignals.searchByPlaceBtnClicked()
            }
        }
    }
}
