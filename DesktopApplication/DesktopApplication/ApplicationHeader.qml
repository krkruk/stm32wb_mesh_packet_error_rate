import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ToolBar {
    id: toolbar
    property string title: "Hello world!"
    property int logState: Qt.Unchecked
    property bool enableLogging: false

    signal clicked

    function checkStateChanged(state) {
        logState = state
    }

    RoundButton {
        id: drawerButton
        text: "\u2630 Serials"
        radius: header.height * 0.5
        onClicked: toolbar.clicked()
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

    CheckBox {
        text: "LOG"
        checkState: logState
        visible: enableLogging
        anchors.right: parent.right
        onCheckStateChanged: logState = checkState
    }
}
