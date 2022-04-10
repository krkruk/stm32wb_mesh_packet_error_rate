import QtQuick 2.12
import QtQuick.Controls 2.12
import stm32.SerialManager 1.0


Rectangle {
    id: delegateButton
    width: parent.width - 1
    height: 90

    Column {
//        spacing: 4
        padding: parent.width / 20
        anchors.fill: parent

        Text {
            text: name
            style: Text.Raised
            font.pixelSize: 18
        }
        Text {
            text: "Vendor:Product=" + vendorIdentifier + ":" + productIdentifier
            x: parent.width / 10
        }
        Text {
            text: "Manufacturer: " + manufacturer
            x: parent.width / 10
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: delegateButton.color = containsMouse ? "grey" : "white"
        onClicked: Serial.onSerialSelected(name)
    }

}
