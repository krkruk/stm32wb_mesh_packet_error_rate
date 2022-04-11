import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0


Item {
    id: areaGetAddress
    width: parent.width


    property string addressResult
    signal clicked(var operation)


    ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.verticalCenter: parent.verticalCenter
        Layout.alignment: Qt.AlignCenter
        width: parent.width

        RowLayout {
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
            onClicked: areaGetAddress.clicked(Stm32SupportedOperations.GET_ADDRESS)
        }

    }

    onAddressResultChanged: addressTextField.text = addressResult
}
