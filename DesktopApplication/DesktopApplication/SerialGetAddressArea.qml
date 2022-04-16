import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0

Rectangle {
    id: areaGetAddress

    property string addressResult
    signal clicked(var operation)

    ColumnLayout {
        width: parent.width
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.verticalCenter: parent.verticalCenter

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            Label {
                id: label
                text: "Result: "
                font.pointSize: 14
            }

            TextField {
                id: addressTextField
                font.pointSize: label.font.pixelSize * 1.2
                horizontalAlignment: Qt.AlignRight
                placeholderText: "...result"
                readOnly: true
                selectByMouse: true
            }
        }

        Button {
            id: buttonGetAddress
            text: "Get Node Address"
            Layout.alignment: Qt.AlignCenter
            onClicked: areaGetAddress.clicked(
                           Stm32SupportedOperations.GET_ADDRESS)
        }
    }

    onAddressResultChanged: addressTextField.text = addressResult
}
