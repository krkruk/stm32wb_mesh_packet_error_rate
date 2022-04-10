import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Item {
    function toggleDrawer() {
        if (drawer.visible) {
            drawer.close()
        } else {
            drawer.open()
        }
    }

    function fillModel() {
        drawerModel.clear()
        var serials = Serial.getSerials()
        for (var i in serials) {
            console.log(serials[i].name)
            drawerModel.append(serials[i])
        }
    }

    Drawer {
        id: drawer
        y: header.height
        width: window.width * 0.45
        height: window.height - header.height
        dragMargin: 0
        onAboutToShow: fillModel()

        ListView {
            id: drawerListView
            height: drawer.height
            width: drawer.width
            clip: true
            model: ListModel {
                id: drawerModel
            }
            delegate: SerialListDelegate {} // onClicked will connect to serial
            footer: Button {
                id: exitButton
                width: parent.width - 1
                text: "Quit"
                onClicked: Qt.quit()
            }
        }
    }
}
