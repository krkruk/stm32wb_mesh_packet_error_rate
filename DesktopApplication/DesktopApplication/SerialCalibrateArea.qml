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
            text: "Measurement time [ms]: "
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
            text: "[ms]"
            font.pointSize: 14
        }

        Button {
            id: buttonGetCalibrate
            text: "Calibrate"
            Layout.alignment: Qt.AlignCenter
            onClicked: areaCalibrate.clicked(
                           Stm32SupportedOperations.CALIBRATE,
                           pingTextField.text, measurementTextField.text)
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
            text: "[ticks/ms]"
            font.pointSize: 14
        }
    }

    onTicksPerMillisResultChanged: resultTicksPerMillis.text = ticksPerMillisResult
}
