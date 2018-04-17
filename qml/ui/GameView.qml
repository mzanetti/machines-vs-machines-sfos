import QtQuick 2.0
import QtQuick.Layouts 1.1
import harbour.machines.vs.machines.sfos.Machines 1.0
import "../components"

Overlay {
    id: root
    property var engine
    property bool fieldOverlay: false
    hideBackground: true

    Rectangle {
        z: 1
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: root.height / 15
        color: app.foregroundColor

        RowLayout {
            id: headerRow
            spacing: headerRow.height / 2
            anchors {
                fill: parent
                leftMargin: headerRow.height / 4
                rightMargin: headerRow.height / 4
            }

            GameLabel {
                text: "LEVEL: " + (root.engine.level + 1)
                font.pixelSize: headerRow.height * 2/3
                color: app.backgroundColor
                Layout.fillWidth: false
            }
            GameLabel {
                text: "WAVE: " + (root.engine.currentWave + 1) + "/" + root.engine.waves
                Layout.fillWidth: true
                font.pixelSize: headerRow.height * 2/3
                color: app.backgroundColor
            }
            Row {
                Image {
                    source: "../graphics/star.svg"
                    height: headerRow.height
                    width: height
                    sourceSize.width: width
                    sourceSize.height: height
                }

                GameLabel {
                    text: root.engine.points
                    font.pixelSize: headerRow.height * 2/3
                    color: app.backgroundColor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row {
                Image {
                    source: "../graphics/dollar.svg"
                    height: headerRow.height
                    width: height
                    sourceSize.width: width
                    sourceSize.height: height
                }

                GameLabel {
                    text: root.engine.money
                    font.pixelSize: headerRow.height * 2/3
                    color: app.backgroundColor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                Image {
                    source: "../graphics/heart.svg"
                    height: headerRow.height
                    width: height
                    sourceSize.width: width
                    sourceSize.height: height
                }
                GameLabel {
                    text: root.engine.lives
                    font.pixelSize: headerRow.height * 2/3
                    color: app.backgroundColor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    BoardView {
        id: boardView
        engine: root.engine
        property int availableVSpace: root.height - headerRow.height - footerRow.height
        property int availableHSpace: root.width
        property bool adjustToWidth: availableHSpace < availableVSpace * 2
        width: adjustToWidth ? availableHSpace : availableVSpace * 2
        height: adjustToWidth ? availableHSpace / 2 : availableVSpace
        anchors.centerIn: parent
        anchors.verticalCenterOffset: (headerRow.height - footerRow.height) / 2
        fieldOverlay: root.fieldOverlay
    }

    Rectangle {
        anchors.fill: footerRow
        color: app.backgroundColor
    }

    RowLayout {
        id: footerRow
        anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
        height: headerRow.height * 2
        IconButton {
            height: headerRow.height * 2
            width: height
            enabled: root.engine.playStatus == Engine.PlayStatusRunning

            iconName: root.engine.playStatus == Engine.PlayStatusRunning ? "pause-button" : "play-button"

            onClicked: {
                root.engine.pauseGame()
            }
        }
        IconButton {
            height: headerRow.height * 2
            width: height
            visible: root.engine.playStatus == Engine.PlayStatusRunning
            iconName: root.engine.gameSpeed == 1 ? "2x-button" : "1x-button"

            onClicked: root.engine.gameSpeed = (root.engine.gameSpeed == 1 ? 2 : 1)
        }

        Item { Layout.fillWidth: true }

        Row {
            id: towersRow
            height: headerRow.height * 2

            property real progress: boardView.selectedField >= 0
                                    && !root.engine.board.field(boardView.selectedField).isOnPath
                                    && root.engine.board.field(boardView.selectedField).towerAllowed
                                    && root.engine.board.field(boardView.selectedField).tower == null
                                    ? 0 : 1

            visible: progress < 1
            Behavior on progress {
                NumberAnimation {}
            }

            transform: [
                Translate {
                    y: towersRow.progress * 8*6
                }
            ]

            Repeater {
                id: towerSelector
                property var baseTower: boardView.selectedField > -1 ? root.engine.board.field(boardView.selectedField).tower : null

                model: engine.availableTowers

                delegate: Item {
                    id: towerDelegate
                    width: height
                    height: headerRow.height * 2

                    Rectangle {
                        color: app.backgroundColor
                        anchors.fill: parent
                    }

                    TowerDelegate {
                        anchors.fill: parent
                        enabled: model.locked ? root.engine.points >= model.unlockPoints : root.engine.money >= model.cost
                        radius: 0
                        source: engine.dataDir + model.image
                        baseSize: model.imageBaseSize
                        spriteCount: model.spriteCount
                        duration: model.animationDuration
                        running: false
                        startFrame: 0
                        locked: model.locked
                        cost: model.cost
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressAndHold: {
                            openTowerInfo();
                        }

                        onClicked: {
                            if (model.locked) {
                                openTowerInfo();
                            } else if (boardView.selectedField > -1 && towerSelector.baseTower === null) {
                                if (engine.addTower(boardView.selectedField, index)) {
                                    boardView.selectedField = -1
                                }
                            }
                        }

                        function openTowerInfo() {
                            towerInfoDialog.lineup = engine.availableTowers.getLineup(index);
                            towerInfoDialog.selectedLevel = 0;
                            towerInfoDialog.shown = true;
                        }
                    }
                }
            }
        }
    }

    GameLabel {
        id: counterLabel
        anchors.centerIn: parent
        text: engine.secsToNextWave
        opacity: 0
        font.pixelSize: root.height / 2
        scale: 3
        color: "#2eacd2"

        onTextChanged: {
            scaleAnimation.start()
        }
        NumberAnimation {
            id: scaleAnimation
            target: counterLabel
            property: "scale"
            from: 0.5
            to: 1.5
        }

        states: [
            State {
                name: "visible"; when: engine.secsToNextWave > 0
                PropertyChanges { target: counterLabel; opacity: 1 }
            }
        ]
        transitions: [
            Transition {
                from: "visible"; to: "*"
                NumberAnimation { property: "opacity"; duration: 500 }
            }

        ]
    }
}


