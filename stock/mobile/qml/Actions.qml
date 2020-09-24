
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
            text: qsTr( "Put Product" )

            onClicked: {
                qmlCppSignals.putBtnClicked()
            }
        }

        Button {
            id: take
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            text: qsTr( "Take Product" )

            onClicked: {
                qmlCppSignals.takeBtnClicked()
            }
        }

        Button {
            id: searchByCode
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            text: qsTr( "Search By Code" )

            onClicked: {
                qmlCppSignals.searchByCodeBtnClicked()
            }
        }

        Button {
            id: searchByPlace
            implicitHeight: appWindow.minimumCtrlHeight
            implicitWidth: parent.width
            text: qsTr( "Search By Place" )

            onClicked: {
                qmlCppSignals.searchByPlaceBtnClicked()
            }
        }
    }
}
