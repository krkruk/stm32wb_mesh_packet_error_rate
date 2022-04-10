import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import stm32.SerialManager 1.0


Item {
    id: areaGetAddress
    width: parent.width

    property string addressResult
    signal clicked(var operation)


    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: buttonGetAddress
            text: "Get Node Address"
            onClicked: areaGetAddress.clicked(Stm32SupportedOperations.GET_ADDRESS)
        }

        TextField {
            id: addressTextField
            width: areaGetAddress.width
            placeholderText: "Result..."
        }
    }

    onAddressResultChanged: addressTextField.text = addressResult
}
