import QtQuick 2.2
import "../../components"
import harbour.machines.vs.machines.sfos.Machines 1.0
import QtQuick.Layouts 1.1

ModalOverlay {
    id: root
    title: "Let us know what you think!"
    showCloseButton: true

    Column {
        anchors.fill: parent
        anchors.margins: app.buttonSize
        spacing: app.margins

        Row {
            anchors { left: parent.left; right: parent.right; margins: app.margins }
            height: parent.height - y

            spacing: app.buttonSize / 2

            PushButton {
                height: parent.height
                width: (parent.width - parent.spacing*2 - orLabel.width) / 2
                color: app.confirmationButtonColor
                onClicked: Qt.openUrlExternally("scope://com.canonical.scopes.clickstore?q=Machines%20vs%20Machines")

                GameLabel {
                    anchors.fill: parent
                    anchors.margins: app.margins
                    wrapMode: Text.WordWrap
                    text: "Leave us a review in the Ubuntu App Store"
                    color: app.buttonTextColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            GameLabel {
                id: orLabel
                anchors.verticalCenter: parent.verticalCenter
                text: "or"
            }

            PushButton {
                height: parent.height
                width: (parent.width - parent.spacing*2 - orLabel.width) / 2
                color: app.confirmationButtonColor
                onClicked: {
                    Qt.openUrlExternally("https://launchpad.net/machines-vs-machines")
                }
                GameLabel {
                    anchors.fill: parent
                    anchors.margins: app.margins
                    wrapMode: Text.WordWrap
                    text: "Visit our Launchpad page to report a bug or contact us."
                    color: app.buttonTextColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
