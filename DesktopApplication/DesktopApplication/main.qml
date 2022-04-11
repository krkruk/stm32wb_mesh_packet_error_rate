import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import stm32.SerialManager 1.0

ApplicationWindow {
    id: window
    title: "STM32WB Packet Error Rate Experiment"
    width: 800
    height: 600
    visible: true
    header: ApplicationHeader {
        id: appHeader
        onClicked: applicationMenu.toggleDrawer()
    }

    ApplicationMenu {
        id: applicationMenu
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: indicator.currentIndex

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
        onCurrentItemChanged: (swipeView.count > 0)
                              && (appHeader.title = currentItem.serialName)

    }

    PageIndicator {
        id: indicator
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        count: swipeView.count
        currentIndex: swipeView.currentIndex
        interactive: true
    }

    Connections {
        target: Serial
        function onSerialConnectionSelected(index, serialName) {
            if (index >= swipeView.count) {
                swipeView.insertItem(index, swipeView.createPage(serialName))
            }
            else {
                swipeView.currentIndex = index
            }
        }
    }
}
