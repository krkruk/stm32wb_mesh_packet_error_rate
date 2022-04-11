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
            id: tabArea
            width: parent.width
            height: parent.height * 0.4
            StackLayout {
                y: tabs.height + 1
                width: parent.width
                height: parent.height
                currentIndex: tabs.currentIndex
                Layout.fillHeight: true
                Layout.fillWidth: true

                SerialGetAddressArea {
                    id: tabAddress
                    onClicked: serial.runCommand(operation, null)
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
                font.family: "Courier"
                font.pointSize: 8
                readOnly: true
                selectByMouse: true
                selectByKeyboard: true
                padding: 5
                color: "#55FF55"
                background: Rectangle {
                    color: "black"
                }

                text: "Log area..."
                wrapMode: TextEdit.WordWrap
            }
        }
    }

    SerialConnector {
        id : serial
        onLogLineReceived: logTextArea.append(line)
        onRunningQuery: {
            console.log("Running query...")
            progressPopup.open()
        }
        onTimeout: {
            console.log("Closing popup")
            progressPopup.close()
        }

        onResultReceived: {
            progressPopup.close()
            console.log("Received a result to be presented in UI [" + result + "] operation id [" + id + "]")

            switch (id) {
               case Stm32SupportedOperations.GET_ADDRESS: {
                   tabAddress.addressResult = result
                   break;
               }
               default: {
                   console.log("Unknown operation")
                   break;
               }
            }
        }

        onError: progressPopup.close()
    }

    Popup {
        id: progressPopup
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: Overlay.overlay
        z: 99

        BusyIndicator { }
    }

    Component.onCompleted: {
        logTextArea.clear()
        serial.open(serialName)
    }
}
