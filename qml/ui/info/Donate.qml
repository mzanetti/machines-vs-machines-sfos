import QtQuick 2.2
import "../../components"
import harbour.machines.vs.machines.sfos.Machines 1.0
import QtQuick.Layouts 1.1

ModalOverlay {
    id: root
    title: "Enjoying the game?"
    showCloseButton: true

    Column {
        anchors.fill: parent
        anchors.margins: app.buttonSize
        spacing: app.margins

        GameLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Send us a small <i><b>thank you</b>!</i>"
        }

        PushButton {
            height: parent.height - y
            width: (parent.width - app.buttonSize * 2)
            anchors.horizontalCenter: parent.horizontalCenter
            color: app.confirmationButtonColor
            onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=USWAUUU2MD9Z4")
            GameLabel {
                anchors.fill: parent
                anchors.margins: app.margins
                wrapMode: Text.WordWrap
                text: "Donate via PayPal"
                color: app.buttonTextColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
