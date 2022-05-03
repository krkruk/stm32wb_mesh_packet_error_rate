import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

ScrollView {
    id: perArea
    clip: true
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn

    property var localCounter: null
    property var remoteCounter: null

    signal runPERClicked(var operation, int interval, int timeout, string srcAddress, string dstAddress)
    signal getPERRemoteResults(var operation, string srcAddress, string dstAddress)

    ColumnLayout {
        Frame {
            Layout.alignment: Qt.AlignCenter
            RowLayout {
                anchors.fill: parent
                TextField {
                    id: localAddressField
                    horizontalAlignment: Qt.AlignRight
                    placeholderText: "Local address [hex]"
                    selectByMouse: true
                    focus: true
                    validator: RegExpValidator {
                        regExp: /[0-9a-fA-F]+/
                    }
                }
                Label {
                    text: "\u27A1"
                    font.pointSize: 14
                }

                TextField {
                    id: remoteAddressField
                    horizontalAlignment: Qt.AlignRight
                    placeholderText: "Remote address [hex]"
                    selectByMouse: true
                    focus: true
                    validator: RegExpValidator {
                        regExp: /[0-9a-fA-F]+/
                    }
                }
            }
        }

        RowLayout {
            Frame {
                id: framePER
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter

                    Label {
                        id: labelPER
                        text: "Test Packet Error Rate"
                        font.pointSize: 14
                    }

                    RowLayout {
        TextField {
            id: timeoutTextField
            horizontalAlignment: Qt.AlignRight
            placeholderText: "Timeout [ms]"
            selectByMouse: true
            focus: true
            validator: IntValidator {}
        }

                        ComboBox {
                            id: intervalsComboBox
                            model: ListModel {
                                id: intervalModel
                            }
                            textRole: "text"
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Button {
                            id: runPERButton
                            text: "Run"
                            onClicked: {
                                var timeoutValue = parseInt(timeoutTextField.text)
                                var intervalCurrentItem = intervalModel.get(intervalsComboBox.currentIndex)
                                var intervalValue = parseInt(!intervalCurrentItem ? 0 : intervalCurrentItem.value)
                                var srcAddress = localAddressField.text
                                var dstAddress = remoteAddressField.text
                                if (!timeoutValue || !intervalValue) {
                                    errorPopup.alert("Timeout/Interval parameters cannot be empty/zero!")
                                    return
                                }
                                if (timeoutValue < 10000) {
                                    errorPopup.alert("'Measurement time' cannot be less than 10,000")
                                    return
                                }
                                if (!srcAddress || !dstAddress) {

                                    errorPopup.alert("Address fields cannot be empty")
                                    return
                                }

                                perArea.runPERClicked(
                                            Stm32SupportedOperations.MEASURE_PER,
                                            intervalValue, timeoutValue,
                                            srcAddress, dstAddress)
                                localCounter = "..."
                            }
                        }
                        TextField {
                            id: localResultTextField
                            Layout.fillWidth: true
                            horizontalAlignment: Qt.AlignRight
                            placeholderText: "Local count result"
                            text: localCounter
                            selectByMouse: true
                            focus: true
                        }
                    }
                }
            }

            Frame {
                contentWidth: framePER.contentWidth
                contentHeight: framePER.contentHeight
                width: framePER.width
                height: framePER.height
                ColumnLayout {
                    anchors.fill: parent
                    Label {
                        text: "Get Remote Results"
                        font.pointSize: 14
                    }

                    RowLayout {
                        Button {
                            text: "Run"
                            onClicked: {
                                var srcAddress = localAddressField.text
                                var dstAddress = remoteAddressField.text
                                if (!srcAddress || !dstAddress) {
                                    errorPopup.alert("Address fields cannot be empty")
                                    return
                                }
                                perArea.getPERRemoteResults(
                                           Stm32SupportedOperations.GET_PER_RESULT,
                                            srcAddress, dstAddress)

                                remoteCounter = "..."
                            }
                        }
                        TextField {
                            id: remoteResultTextField
                            horizontalAlignment: Qt.AlignRight
                            Layout.fillWidth: true
                            placeholderText: "Remote count result"
                            text: remoteCounter
                            selectByMouse: true
                            focus: true
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: errorPopup
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: Overlay.overlay
        z: 99

        property string text: "No Error"
        function alert(msg) {
            errorPopup.text = msg
            errorPopup.open()
        }

        ColumnLayout {
            Label {
                text: errorPopup.text
            }
            Button {
                text: "OK"
                Layout.alignment: Qt.AlignHCenter
                onClicked: errorPopup.close()
            }
        }
    }

    onVisibleChanged: {
        var connSettings = Serial.getConnectionSettings()
        intervalModel.clear()
        intervalModel.append({
                                 "text": "Select interval",
                                 "value": 0
                             })
        for (var key in connSettings) {
            intervalModel.append({
                                     "text": key + "[ms]",
                                     "value": connSettings[key]
                                 })
        }
        intervalsComboBox.currentIndex = 0
    }
}
