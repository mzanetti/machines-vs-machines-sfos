import QtQuick 2.0
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0

Overlay {
    id: root
    color: app.backgroundColor

    property var engine
    property int level: engine ? engine.level : 0
    property int points: engine ? engine.earnedPoints : 0
    property int maxPoints: engine && engine.levelPack ? engine.levelPack.get(engine.level).rewardPoints : 0
    property int highScore: engine && engine.levelPack ? engine.levelPack.get(engine.level).highscore : 0
    property bool success: engine ? engine.playStatus == Engine.PlayStatusWon : false

    signal retry();

    GameLabel {
        anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: app.margins }
        font.pixelSize: app.titleSize
        text: "LEVEL " + (root.level + 1)
    }

    Column {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -app.buttonSize / 2
        spacing: app.margins

        Image {
            id: image
            anchors { horizontalCenter: parent.horizontalCenter}
            height: root.height / 4
            // ssw : w = ssh : h
            width: sourceSize.width * height / sourceSize.height
            source: root.success && root.maxPoints > 0 ?
                        "../graphics/three-stars-" + Math.floor(3 * root.points / root.maxPoints) + ".svg" :
                        "../graphics/star-fail.svg"
        }

        GameLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: app.hugeFontSize
            text: root.success ? (root.points == root.highScore ? "New Best Score: " : "Score: ") + root.points + "/" + root.maxPoints : "Failed"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
    }

    PushButton {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: app.margins
        }
        text: "Okay"
        height: app.buttonSize
        width: app.buttonSize * 3
        color: app.confirmationButtonColor
        onClicked: {
            if (engine.totalPoints === engine.levelPack.totalPoints || SettingsBackend.allUnlocked) {
                finishImage.shown = true
            } else {
                engine.stopGame();
            }
        }
        textColor: app.buttonTextColor
    }
    PushButton {
        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: app.margins
        }
        text: "Retry"
        height: app.buttonSize
        width: app.buttonSize * 3
        color: app.playButtonColor
        onClicked: root.retry();
        textColor: app.buttonTextColor
    }

    Image {
        id: finishImage
        property bool shown: false
        width: parent.width * 2
        height: width / 2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: finishImage.shown ? 0 : -finishImage.height
        source: engine.dataDir + "/" + engine.levelPack.id + "/finish-"
                + (engine.difficulty == Engine.DifficultyEasy ? "easy" : engine.difficulty == Engine.DifficultyMedium ? "medium" : "hard")
                + ".png"
        sourceSize.width: width
        sourceSize.height: height

        Behavior on anchors.verticalCenterOffset {
            NumberAnimation { }
        }

        PushButton {
            anchors {
                right: parent.right
                rightMargin: app.margins + finishImage.width / 4
                bottom: parent.bottom
                bottomMargin: app.margins + (finishImage.height - root.height) / 2
            }
            text: "Okay"
            height: app.buttonSize
            width: app.buttonSize * 3
            color: app.confirmationButtonColor
            onClicked: {
                finishImage.shown = false
                engine.stopGame();
            }
            textColor: app.buttonTextColor
        }
    }
}
