import QtQuick 2.0
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0

ModalOverlay {
    id: scoreInfo

    GameLabel {
        id: titleLabel
        anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: app.margins }
        font.pixelSize: app.titleSize
        text: engine.levelPack.name
    }

    GameLabel {
        anchors {
            top: titleLabel.bottom
            horizontalCenter: titleLabel.horizontalCenter
        }
        text: engine.difficulty == Engine.DifficultyEasy ? "easy"
            : engine.difficulty == Engine.DifficultyMedium ? "medium" : "hard"
        color: engine.difficulty == Engine.DifficultyEasy ? "#27ae60"
             : engine.difficulty == Engine.DifficultyMedium ? "#e67e22" : "#c0392b"
        font.pixelSize: app.titleSize * 0.8
    }


    Column {
        anchors.centerIn: parent

        GameLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: app.hugeFontSize
            color: app.playButtonColor
            text: "Levels: " + engine.highestUnlockedLevel + "/" + engine.levelPack.count
        }

        GameLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: app.hugeFontSize
            color: app.playButtonColor
            text: "Stars: " + engine.totalPoints + "/" + engine.levelPack.totalPoints
        }
    }


    PushButton {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: height / 2
        height: app.buttonSize
        width: app.buttonSize * 3
        color: app.rejectButtonColor
        text: "Reset"
        textColor: app.buttonTextColor

        onClicked: {
            engine.resetScores();
            scoreInfo.closeButtonClicked()
        }
    }
    PushButton {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: height / 2
        height: app.buttonSize
        width: app.buttonSize * 3
        color: app.confirmationButtonColor
        text: "Okay"
        textColor: app.buttonTextColor

        onClicked: {
            scoreInfo.closeButtonClicked()
        }
    }
}
