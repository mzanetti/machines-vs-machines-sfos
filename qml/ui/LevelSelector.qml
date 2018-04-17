import QtQuick 2.0
import harbour.machines.vs.machines.sfos.Machines 1.0
import "../components"

Overlay {
    id: root
    autoClose: false

    property var engine
    property var levelPack: engine.levelPack != null ? engine.levelPack : null

    signal back()
    signal levelSelected(int index)
    signal showScoreInfo()

    AnimatedDelegate {
        anchors { right: parent.right; bottom: parent.bottom }
        height: root.height * .75
        width: height
        property real rand: root.levelPack ? Math.random() : 0
        property bool towerOrEnemy: rand < 0.5
        property int index: root.levelPack ? (Math.random() * (towerOrEnemy ? root.levelPack.towers.count : root.levelPack.enemies.count)) : -1
        property var backgroundItem: root.levelPack ? (towerOrEnemy ? root.levelPack.towers.get(index, 0) : root.levelPack.enemies.get(index)) : null
        source: backgroundItem ? engine.dataDir + backgroundItem.image : ""
        baseSize: backgroundItem ? backgroundItem.imageBaseSize : 1
        frameCount: backgroundItem ? backgroundItem.spriteCount : 1
        duration: 5000
        running: false
        spriteSet: 0
        startFrame: 0
        opacity: .2
        useEngineClock: false
    }

    GameLabel {
        id: titleLabel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: app.margins
        }
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: app.titleSize
        text: root.levelPack ? root.levelPack.name : ""
        font.weight: Font.DemiBold
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

    Row {
        id: scoreRow
        anchors { right: parent.right; bottom: parent.bottom; margins: app.margins }
        height: app.fontSize * 2
        Image {
            source: "../graphics/dark-star.svg"
            height: parent.height
            width: height
            sourceSize.width: width
            sourceSize.height: height
        }

        GameLabel {
            text: root.engine.points
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors.fill: scoreRow
        onClicked: {
            root.showScoreInfo();
        }
    }

    Flickable {
        id: levelGrid
        property int baseSize: root.height / 5
        property int columns: 3
        property int rows: 2

        anchors {
            fill: parent
            leftMargin: app.buttonSize / 2
            rightMargin: app.buttonSize / 2
            topMargin: levelGrid.baseSize / 2
            bottomMargin: levelGrid.baseSize
        }

        width: parent.width / 2
        height: width *2 / 3
        contentWidth: pagesRow.width

        property int currentIndex: 0
        onCurrentIndexChanged: {
            SettingsBackend.lastUsedLevelPage = currentIndex;
        }
        Component.onCompleted: {
            currentIndex = SettingsBackend.lastUsedLevelPage;
            contentX = currentIndex * (levelGrid.width + pagesRow.spacing);
        }

        onDragEnded: {
            var index = 0;


            if (horizontalVelocity > 400) {
                index = Math.min(pagesRepeater.count - 1, currentIndex + 1)
            } else if (horizontalVelocity < -400) {
                index = Math.max(0, currentIndex - 1)
            } else {
                index = Math.round(contentX / (width + pagesRow.spacing))
            }

            currentIndex = index;

            snapAnimation.to = index * (levelGrid.width + pagesRow.spacing)
            snapAnimation.start()
        }

        NumberAnimation {
            id: snapAnimation
            target: levelGrid
            property: "contentX"
            duration: 333
        }

        Row {
            id: pagesRow
            spacing: app.buttonSize * 2

            Repeater {
                id: pagesRepeater
                model: Math.ceil(engine.levelCount / (levelGrid.columns * levelGrid.rows))
                LevelSelectorPage {
                    height: levelGrid.height
                    width: levelGrid.width
                    columns: levelGrid.columns
                    rows: levelGrid.rows
                    baseSize: levelGrid.baseSize
                    levelPack: engine.levelPack
                    start: index * columns * rows

                    onLevelSelected: root.levelSelected(index)
                }
            }
        }
    }

    Row {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: app.buttonSize / 3
        }
        visible: pagesRepeater.count > 1

        Repeater {
            model: pagesRepeater.count
            delegate: MouseArea {
                property bool active: levelGrid.currentIndex == index
                width: app.buttonSize / 3 * 2
                height: width
                Rectangle {
                    anchors.centerIn: parent
                    width: app.buttonSize / (parent.active ? 2 : 3)
                    height: width
                    color: app.textColor
                    radius: width / 2
                    Behavior on width {
                        NumberAnimation { duration: 333 }
                    }
                }
                onClicked: {
                    levelGrid.currentIndex = index;
                    snapAnimation.to = index * (levelGrid.width + pagesRow.spacing)
                    snapAnimation.start()
                }
            }
        }
    }
}
