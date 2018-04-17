import QtQuick 2.2
import "../../components"
import harbour.machines.vs.machines.sfos.Machines 1.0
import QtQuick.Layouts 1.1

ModalOverlay {
    id: root
    title: "Credits"
    showCloseButton: true

    Flickable {
        anchors.fill: parent
        anchors.margins: app.margins
        contentHeight: columnLayout.height
        clip: true
        ColumnLayout {
            id: columnLayout
            width: parent.width
            spacing: app.margins

            GameLabel {
                font.pixelSize: app.titleSize
                font.bold: true
                text: qsTr("Game development")
            }

            GameLabel {
                text: "<b>" + qsTr("Development") + ":</b> Michael Zanetti"
            }
            GameLabel {
                text: "<b>" + qsTr("Design/Artwork") + ":</b> Michał Prędotka"
            }

            Rectangle { width: parent.width; height: 1; color: "black" }

            GameLabel {
                font.pixelSize: app.titleSize
                font.bold: true
                text: qsTr("Level packs")
            }

            Repeater {
                model: engine.levelPacks

                ColumnLayout {
                    width: parent.width

                    GameLabel {
                        Layout.fillWidth: true
                        text: model.name
                        font.bold: true
                    }
                    GameLabel {
                        Layout.fillWidth: true
                        text: model.copyright
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
}
