import QtQuick 2.0
import "../components"

ModalOverlay {
    id: levelSplash
    property int level: -1

    signal startLevel(int level)

    GameLabel {
        anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: app.margins }
        font.pixelSize:  app.titleSize
        text: "LEVEL " + (levelSplash.level + 1)
        color: app.textColor
    }

    Column {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -app.buttonSize / 2
        spacing: levelSplash.height / 25

        property var level: levelSplash.level > -1 ? engine.levelPack.get(levelSplash.level) : null
        property int starCount: level != null ? Math.floor(3 * level.highscore / level.rewardPoints) : 0

        Image {
            height: levelSplash.height / 4
            // ss.w : ss.h = w : h
            width: sourceSize.width * height / sourceSize.height
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../graphics/three-stars-" + parent.starCount + ".svg"
        }
        GameLabel {
            text: "Best Score: " + (parent.level ? parent.level.highscore + "/" + parent.level.rewardPoints : "0/0")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: app.hugeFontSize
        }
    }

    PushButton {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height / 2
        height: app.buttonSize
        width: app.buttonSize * 3
        color: app.playButtonColor
        text: "Play"
        textColor: app.buttonTextColor

        onClicked: {
            levelSplash.startLevel(levelSplash.level);
            levelSplash.closeButtonClicked()
        }
    }

    MouseArea {
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        height: app.buttonSize * 2
        width: app.buttonSize * 2
        Image {
            anchors.fill: parent
            source: "../graphics/levels-view-button.svg"
        }
        onClicked: {
            levelSplash.closeButtonClicked()
        }
    }
}
