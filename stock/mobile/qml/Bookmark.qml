
import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
	id: root

    width: minimumCtrlHeight
    height: minimumCtrlHeight

    signal clicked()

    Image {
        width: parent.width - minimumCtrlHeight / 4
        height: parent.height - minimumCtrlHeight / 4
        anchors.centerIn: root
        source: "qrc:/img/bookmark_48x48.png"
    }

    MouseArea {
        anchors.fill: parent

        onClicked: root.clicked()
    }
}
