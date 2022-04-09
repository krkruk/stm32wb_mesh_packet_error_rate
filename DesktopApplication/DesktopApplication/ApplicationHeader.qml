import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


ToolBar {
    id: toolbar
    property string title: "Hello world!"

    RoundButton {
        id: drawerButton
        text: "\u2630 Serials"
        radius: header.height * 0.5
        onClicked: drawer.visible ? drawer.close() : drawer.open()
    }

    Label {
        id: headerLabel
        text: title
        width: parent.width - drawerButton.width
        height: parent.height
        x: drawerButton.width
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}

