import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

ApplicationWindow {
    id: window
    title: "STM32WB Packet Error Rate Experiment"
    width: 800
    height: 600
    visible: true
    header: ApplicationHeader {
        id: appHeader
    }

    function fillModel() {
        drawerModel.clear()
        var serials = Serial.getSerials()
        for (var i in serials) {
            console.log(serials[i].name)
            drawerModel.append(serials[i])
        }
    }

    SwipeView {
        id: swipeView
        y: appHeader.height
        width: parent.width
        height: parent.header - appHeader.height
        anchors.fill: parent

        function addPage(page) {
            addItem(page)
            page.visible = true
        }

        function createPage(title) {
            var component = Qt.createComponent("SerialMainComponent.qml")
            var page = component.createObject(swipeView, {
                                                  "serialName": title
                                              })
            appHeader.title = title
            return page
        }
        onCurrentItemChanged: appHeader.title = currentItem.serialName
    }

    PageIndicator {
        id: indicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
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
                width: parent.width
                text: "Quit"
                onClicked: Qt.quit()
            }
        }
    }

    Connections {
        target: Serial
        onSerialConnectionSelected: {
            console.log("i=" + index + " port = " + serialName)
            swipeView.insertItem(index, swipeView.createPage(serialName))
        }
    }

    Popup {
        id: quitBoxPopup
        anchors.centerIn: overlay
        contentItem: Text {
            text: "QUITTING..."
        }
    }
}
