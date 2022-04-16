import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Rectangle {
    id: areaCalibrate

    property string ticksPerMillisResult
    signal clicked(var operation, int pingMs, int timeout)

    GridLayout {
        columns: 3
        anchors.centerIn: parent

        Label {
            id: labelPing
            text: "Ping interval: "
            font.pointSize: 14
        }

        TextField {
            id: pingTextField
            font.pointSize: labelPing.font.pixelSize * 1.2
            horizontalAlignment: Qt.AlignRight
            placeholderText: "...result"
            selectByMouse: true
            focus: true
            validator: IntValidator {
                id: pingValidator
                bottom: 1
                top: (65535 * .9) / 2 // 2 bytes/16bits minus some threshold
            }
        }

        Label {
            text: "[ms]"
            font.pointSize: 14
        }

        Label {
            id: labelTimeout
            text: "Measurement time: "
            font.pointSize: 14
        }

        TextField {
            id: measurementTextField
            font.pointSize: labelTimeout.font.pixelSize * 1.2
            horizontalAlignment: Qt.AlignRight
            placeholderText: "...result"
            selectByMouse: true
            focus: true
            validator: IntValidator {
                id: measurementValidator
                bottom: 10000
                top: 0xfffffff // 32 bits
            }
        }

        Label {
            text: "[ticks]"
            font.pointSize: 14
        }

        Button {
            id: buttonGetCalibrate
            text: "Calibrate"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                var pingValue = parseInt(pingTextField.text)
                var measurementValue = parseInt(measurementTextField.text)
                if (!pingValue || !measurementValue) {
                    errorPopup.text = "Fields cannot be empty"
                    errorPopup.open()
                    return
                }

                if (measurementValue < measurementValidator.bottom || measurementValue > measurementValidator.top) {
                    errorPopup.text = "'Measurement time' must be within range [" + measurementValidator.bottom + ", " + measurementValidator.top + "]"
                    errorPopup.open()
                    return
                }
                if (pingValue < pingValidator.bottom || pingValue > pingValidator.top) {
                    errorPopup.text = "'Ping interval' must be within range [" + pingValidator.bottom + ", " + pingValidator.top + "] ms"
                    errorPopup.open()
                    return
                }

                areaCalibrate.clicked(Stm32SupportedOperations.CALIBRATE,
                                      pingTextField.text, measurementValue)
            }
        }

        TextField {
            id: resultTicksPerMillis
            font.pointSize: ticksPerMillisLabel.font.pixelSize * 1.2
            horizontalAlignment: Qt.AlignRight
            placeholderText: "...result"
            readOnly: true
            selectByMouse: true
        }

        Label {
            id: ticksPerMillisLabel
            text: "[ticks/ping interval]"
            font.pointSize: 14
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

    onTicksPerMillisResultChanged: resultTicksPerMillis.text = ticksPerMillisResult
}
