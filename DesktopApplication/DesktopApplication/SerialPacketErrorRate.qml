import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Item {
    id: perArea
    width: parent.width

    signal runPERClicked(var operation, int interval, int timeout)
    signal getPERRemoteResults(var operation)

    ColumnLayout {

        RowLayout {
            Label {
                id: labelPER
                text: "Test PER"
                font.pointSize: 14
            }

            TextField {
                id: timeoutTextField
                font.pointSize: labelPER.font.pixelSize * 1.2
                horizontalAlignment: Qt.AlignRight
                placeholderText: "Timeout [ticks]"
                selectByMouse: true
                focus: true
                validator: IntValidator {
                    bottom: 1
                    top: 65535
                }
            }
            ComboBox {
                id: intervalsComboBox
                model: ListModel {
                    id: intervalModel
                }
                textRole: "text"
            }

            Button {
                text: "Run"
                onClicked: {
                    var timeoutValue = parseInt(timeoutTextField.text)
                    var intervalCurrentItem = intervalModel.get(intervalsComboBox.currentIndex)
                    var intervalValue = parseInt(!intervalCurrentItem ? 0 : intervalCurrentItem.value)
                    console.log("TimeoutValue=" + timeoutValue + " IntervalValue=" + intervalValue)
                    if (!timeoutValue || !intervalValue) {
                        errorPopup.text = "Fields cannot be empty!"
                        errorPopup.open()
                        return
                    }
                    if (timeoutValue < 10000) {
                        errorPopup.text = "'Measurement time' cannot be less than 10,000"
                        errorPopup.open()
                        return
                    }
                    perArea.runPERClicked(Stm32SupportedOperations.MEASURE_PER,
                                          intervalValue, timeoutValue)
                }
            }
        }

        RowLayout {
            Label {
                text: "Get remote data"
                font.pointSize: 14
            }

            Button {
                text: "Run"
                onClicked: perArea.getPERRemoteResults(Stm32SupportedOperations.GET_PER_RESULT)
            }
        }

        RowLayout {
            Label {
                id: labelLocalCount
                text: "Local count:  "
                font.pointSize: 14
            }
            TextField {
                id: localResutlTextField
                font.pointSize: labelLocalCount.font.pixelSize * 1.2
                horizontalAlignment: Qt.AlignRight
                placeholderText: "...result"
                selectByMouse: true
                focus: true
            }
        }

        RowLayout {
            Label {
                id: labelRemoteCount
                text: "Remote count:"
                font.pointSize: 14
            }
            TextField {
                id: remoteResultTextField
                font.pointSize: labelLocalCount.font.pixelSize * 1.2
                horizontalAlignment: Qt.AlignRight
                placeholderText: "...result"
                selectByMouse: true
                focus: true
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
        for (var key in connSettings) {
            intervalModel.append({
                                     "text": key + "[ms]",
                                     "value": connSettings[key]
                                 })
        }
    }
}
