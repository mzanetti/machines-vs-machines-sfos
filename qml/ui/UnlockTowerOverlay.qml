import QtQuick 2.0
import QtQuick.Layouts 1.1
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0

ModalOverlay {
    id: root
    color: "#77000000"

    property var lineup: null
    property int selectedLevel: 0


    Column {
        id: optionColumn
        anchors { left: parent.left; right: parent.right; top: parent.top; margins: app.margins }
        spacing: app.margins

        GameLabel {
            text: root.lineup ? root.lineup[root.selectedLevel].name : ""
            font.pixelSize: app.titleSize
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: app.buttonSize / 2
            Repeater {
                model: root.lineup

                delegate: MouseArea {
                    height: app.buttonSize * 2
                    width: app.buttonSize * 2
                    onClicked: root.selectedLevel = index

                    TowerDelegate {
                        anchors.fill: parent
                        source: engine.dataDir + root.lineup[index].image
                        baseSize: root.lineup[index].imageBaseSize
                        spriteCount: root.lineup[index].spriteCount
                        duration: root.lineup[index].animationDuration
                        running: false
                        startFrame: 0
                        locked: root.lineup[index].locked
                        //cost: root.lineup[index].cost

                        opacity: index == root.selectedLevel ? 1 : 0.6
                    }
                }

            }
        }


        Row {
            anchors { left: parent.left; right: parent.right }
            spacing: app.margins * 4

            ColumnLayout {
                width: (parent.width - parent.spacing) / 2
                RowLayout {
                    GameLabel {
                        text: "Damage:"
                        Layout.fillWidth: true
                    }
                    GameLabel {
                        text: root.lineup ? root.lineup[root.selectedLevel].damage : ""
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }
                }
                RowLayout {
                    GameLabel {
                        text: "Slowdown:"
                    }
                    GameLabel {
                        text: root.lineup ?  root.lineup[root.selectedLevel].slowdown : ""
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }
                }
                RowLayout {
                    GameLabel {
                        text: "Radius:"
                    }
                    GameLabel {
                        text: root.lineup ? root.lineup[root.selectedLevel].radius : ""
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
            ColumnLayout {
                width: (parent.width - parent.spacing) / 2

                RowLayout {
                    GameLabel {
                        text: "Shot duration:"
                    }
                    GameLabel {
                        text: root.lineup ? root.lineup[root.selectedLevel].shotDuration +  " ms" : ""
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }
                }
                RowLayout {
                    GameLabel {
                        text: "Shot recovery:"
                    }
                    GameLabel {
                        text: root.lineup ? root.lineup[root.selectedLevel].shotRecovery + " ms" : ""
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }
                }
                RowLayout {
                    GameLabel {
                        text: "Cost:"
                    }
                    GameLabel {
                        text: root.lineup ? root.lineup[root.selectedLevel].cost + " $" : ""
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }

    RowLayout {
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; margins: app.margins }
        spacing: app.margins
        PushButton {
            text: root.lineup ?
                      (enabled ? "Unlock tower (" + root.lineup[root.selectedLevel].unlockPoints + " ⚝)"
                          : "Get " + (root.lineup[root.selectedLevel].unlockPoints - engine.points) + " more ⚝ to unlock!")
                    : ""
            enabled: root.lineup ? engine.points >= root.lineup[root.selectedLevel].unlockPoints : false
            color: enabled ? app.confirmationButtonColor : app.cancelButtonColor
            opacity: root.lineup ? (root.lineup[root.selectedLevel].locked ? 1 : 0) : 0
            onClicked: {
                engine.unlockTower(root.lineup[root.selectedLevel].index, root.lineup[root.selectedLevel].level)
                root.closeButtonClicked()
            }
        }
        Item {
            height: parent.height
            Layout.fillWidth: true
        }

        PushButton {
            text: "Close"
            color: app.playButtonColor
            width: app.buttonSize * 3
            onClicked: {
                root.closeButtonClicked()
            }
        }
    }
}

