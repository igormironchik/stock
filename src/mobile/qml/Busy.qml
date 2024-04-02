
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        running: true
    }
}
