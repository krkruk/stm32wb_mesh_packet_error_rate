import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Item {
    id: areaCalibrate
    width: parent.width

    property string ticksPerMillisResult
    signal clicked(var operation, int pingMs, int timeout)

    GridLayout {
        columns: 3
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width

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
                bottom: 1
                top: 65535
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
                bottom: 1
                top: 65535
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
                var measurementValue = parseInt(measurementTextField.text);
                if (!pingValue || !measurementValue) {
                    errorPopup.text = "Fields cannot be empty"
                    errorPopup.open()
                    return
                }

                if (measurementValue < 10000) {
                    errorPopup.text = "'Measurement time' cannot be less than 10,000"
                   errorPopup.open()
                    return
                }
                if (pingValue < 1) {
                    errorPopup.text = "'Ping interval' cannot be less than 1"
                   errorPopup.open()
                    return
                }

                areaCalibrate.clicked(
                           Stm32SupportedOperations.CALIBRATE,
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
