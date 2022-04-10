import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Rectangle {
    id: serialMainComponent
    height: parent.height

    property string serialName: "unknown"

    Column {
        anchors.fill: parent
        Rectangle {
            width: parent.width
            height: parent.height * 0.4
            StackLayout {
                y: tabs.height + 1
                width: parent.width
                currentIndex: tabs.currentIndex
                Item {
                    id: tabAddress
                    Label {
                    text: "Get Node Address"
                    }
                }
                Item {
                    id: tabCalibrate
                }
                Item {
                    id: tabPacketErrorRate
                }
            }

            TabBar {
                id: tabs
                width: parent.width

                TabButton {
                    text: qsTr("Address")
                }
                TabButton {
                    text: qsTr("Calibrate")
                }
                TabButton {
                    text: qsTr("Packet Error Rate")
                }
            }

        }

        ScrollView {
            width: parent.width
            height: parent.height * 0.6

            TextArea {
                id: logTextArea
                background: Rectangle {
                    anchors.fill: parent
                    color: "black"
                }
                color: "#55FF55"
                font.family: "Courier"
                readOnly: true
                text: "Log area..."
                padding: 4
            }
        }
    }
}
