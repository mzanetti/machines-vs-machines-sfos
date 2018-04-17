import QtQuick 2.0
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0


Overlay {
    id: root
    property bool showDeveloperOptions: false
    showHomeButton: false

    readonly property string currentLevelpackId: priv.currentLevelPackId
    signal loadLevelPack(string levelPack)
    signal openSettings()
    signal openInfoPage()

    QtObject {
        id: priv
        property string currentLevelPackId
    }

    LevelPackCarousel {
        id: listView
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: root.width / 16
        }
        height: parent.height / 2
        width: height * 2
        model: engine.levelPacks
        currentIndex: SettingsBackend.lastUsedLevelPack

        onCurrentIndexChanged: {
            SettingsBackend.lastUsedLevelPack = currentIndex
        }

        Binding {
            target: priv
            property: "currentLevelPackId"
            value: listView.currentLevelPackId
        }
    }

    PushButton {
        anchors { bottom: parent.bottom; bottomMargin: app.margins; horizontalCenter: parent.horizontalCenter }
        text: "Play!"
        color: app.playButtonColor
        textColor: app.buttonTextColor
        height: app.buttonSize
        width: app.buttonSize * 3
        onClicked: {
            root.loadLevelPack(listView.currentLevelPackId)
        }
    }

    DifficultySelector {
        id: difficultySelector
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: listView.bottom
            topMargin: app.margins
        }
        width: app.buttonSize * 4
        height: app.buttonSize * 1.5
        difficulty: engine.difficulty
        onClicked: engine.difficulty = difficulty
    }

    IconButton {
        id: settingsButton
        anchors { left: parent.left; bottom: parent.bottom; margins: 8 }
        height: app.buttonSize
        width: app.buttonSize
        iconSource: root.showDeveloperOptions ? "../graphics/settings.svg" : "../graphics/sound-settings.svg"
        hasPressedState: false
        onClicked: {
            root.openSettings()
        }
    }
    IconButton {
        id: infoButton
        anchors { left: settingsButton.right; bottom: parent.bottom; margins: 8 }
        height: app.buttonSize
        width: app.buttonSize
        iconSource: "../graphics/tower-info.svg"
        hasPressedState: false
        onClicked: {
            root.openInfoPage();
        }
    }
}
