import QtQuick 2.0
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0

ModalOverlay {
    id: root
    autoClose: false
    property int level: 0
    title: "LEVEL " + (root.level + 1)

    onBackgroundClicked: engine.resumeGame();

    Column {
        id: optionColumn
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: childrenRect.width
        spacing: root.height / 30

        ListItems {
            text: "Restart level"
            iconSource: "../graphics/reset-button.svg"
            height: root.height / 8
            onClicked: engine.restartLevel();
        }

        ListItems {
            text: "Back to level selection"
            iconSource: "../graphics/levels-view-button.svg"
            height: root.height / 8
            onClicked: engine.stopGame();
        }

        ListItems {
            text: "Back to main menu"
            iconSource: "../graphics/main-menu-button.svg"
            height: root.height / 8
            onClicked: {
                engine.stopGame();
                engine.unloadLevelPack();
            }
        }
    }

    Rectangle {
        width: resumeButtonImage.width * 1.4
        height: resumeButtonImage.width * 1.4
        radius: height / 2
        color: app.backgroundColor
        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: -width / 5
        }

        Image {
            id: resumeButtonImage
            anchors.centerIn: parent
            height: root.height / 8
            width: root.height / 8
            source: resumeButton.pressed ? "../graphics/play-button-pushed.svg" : "../graphics/play-button.svg"
            sourceSize.width: width
            sourceSize.height: height

            MouseArea {
                id: resumeButton
                anchors.fill: parent
                onClicked: {
                    engine.resumeGame();
                }
            }
        }
    }


    Rectangle {
        width: musicButton.width * 1.4
        height: musicButton.width * 1.4
        radius: height / 2
        color: app.backgroundColor
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: -width / 5
        }

        Image {
            id: musicButton
            anchors.centerIn: parent
            height: root.height / 8
            width: root.height / 8
            source: SettingsBackend.audioEnabled ?
                        (buttonArea.pressed ? "../graphics/sound-off-button-pushed.svg" : "../graphics/sound-off-button.svg")
                      : (buttonArea.pressed ? "../graphics/sound-on-button-pushed.svg" : "../graphics/sound-on-button.svg")
            sourceSize.width: width
            sourceSize.height: height

            MouseArea {
                id: buttonArea
                anchors.fill: parent
                onClicked: {
                    SettingsBackend.audioEnabled = !SettingsBackend.audioEnabled
                }
            }
        }
    }
}
