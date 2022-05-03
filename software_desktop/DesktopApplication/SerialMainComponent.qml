import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Rectangle {
    id: serialMainComponent
    height: parent.height

    property string serialName: "unknown"
    property int logAreaSwitch: Qt.Unchecked

    signal logSwitched(var state)

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
                    onClicked: {
                        logsModel.clear()
                        serial.runCommand(operation, null)
                    }
                }

                SerialCalibrateArea {
                    id: tabCalibrate
                    onClicked: {
                        logsModel.clear()
                        serial.runCommand(operation, {
                                              "pingIntervalMs": pingMs,
                                              "timeoutMs": timeout
                                          })
                    }
                }
                SerialPacketErrorRate {
                    id: tabPacketErrorRate
                    onRunPERClicked: {
                        logsModel.clear()
                        serial.runCommand(operation, {
                                              "interval": interval,
                                              "timeout": timeout,
                                              "srcAddress": srcAddress,
                                              "dstAddress": dstAddress
                                          })
                    }
                    onGetPERRemoteResults: {
                        logsModel.clear()
                        serial.runCommand(operation, {
                                              "srcAddress": srcAddress,
                                              "dstAddress": dstAddress
                                          })
                    }
                }
            }
        }

        Rectangle {
            height: parent.height * 0.6
            width: parent.width
            color: "black"
            ListView {
                id: logsListView
                anchors.fill: parent
                clip: true
                model: ListModel {
                    id: logsModel
                    ListElement {
                        value: "Log area..."
                    }
                }
                delegate: Text {
                    font.family: "Courier"
                    font.pointSize: 8
                    leftPadding: 5

                    color: "#55FF55"
                    wrapMode: Text.WordWrap
                    text: value
                }
                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                ScrollBar.horizontal: ScrollBar {
                    active: true
                }

                function appendAndScroll(line) {
                    logsModel.append({
                                         "value": line
                                     })
                    logsListView.currentIndex = logsModel.count - 1
                }
            }
        }
    }

    SerialConnector {
        id: serial
        onLogLineReceived: logsListView.appendAndScroll(line)
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
                break
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

    onLogAreaSwitchChanged: {
        logSwitched(logAreaSwitch)
        serial.enableLogArea = logAreaSwitch != 0
    }

    Component.onCompleted: {
        serial.open(serialName)
    }
}
