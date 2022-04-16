import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Rectangle {
    id: serialMainComponent
    height: parent.height

    property string serialName: "unknown"

    Column {
        id: column
        anchors.fill: parent

        Rectangle {
            height: parent.height * 0.4
            width: parent.width
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

            StackLayout {
                y: tabs.height + 1
                height: parent.height - tabs.height
                width: parent.width
                currentIndex: tabs.currentIndex

                SerialGetAddressArea {
                    id: tabAddress
                    onClicked: serial.runCommand(operation, null)
                }

                SerialCalibrateArea {
                    id: tabCalibrate
                    onClicked: {
                        serial.runCommand(operation, {
                                              "pingIntervalMs": pingMs,
                                              "timeoutMs": timeout
                                          })
                    }
                }
                SerialPacketErrorRate {
                    id: tabPacketErrorRate
                    onRunPERClicked: {
                        serial.runCommand(operation, {
                                              "interval": interval,
                                              "timeout": timeout,
                                              "srcAddress": srcAddress,
                                              "dstAddress": dstAddress
                                          })
                    }
                    onGetPERRemoteResults: {
                        serial.runCommand(operation, {
                                              "srcAddress": srcAddress,
                                              "dstAddress": dstAddress
                                          })
                    }
                }
            }
        }

        ScrollView {
            height: parent.height * 0.6
            width: parent.width

            TextArea {
                id: logTextArea
                clip: true
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

                function appendAndScroll(line) {
                    logTextArea.append(line)
                    logTextArea.cursorPosition = logTextArea.length - 1
                }
            }
        }
    }

    SerialConnector {
        id: serial
        onLogLineReceived: logTextArea.appendAndScroll(line)
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
            console.log("Received a result to be presented in UI [" + result
                        + "] operation id [" + id + "]")

            switch (id) {
            case Stm32SupportedOperations.GET_ADDRESS:
            {
                tabAddress.addressResult = result
                break
            }
            case Stm32SupportedOperations.CALIBRATE:
            {
                tabCalibrate.ticksPerMillisResult = result
                break
            }
            case Stm32SupportedOperations.MEASURE_PER:
            {
                tabPacketErrorRate.localCounter = result
                break
            }
            case Stm32SupportedOperations.GET_PER_RESULT:
            {
                tabPacketErrorRate.remoteCounter = result
                break;
            }
            default:
            {
                console.log("Unknown operation")
                break
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

        BusyIndicator {}
    }

    Component.onCompleted: {
        logTextArea.clear()
        serial.open(serialName)
    }
}
