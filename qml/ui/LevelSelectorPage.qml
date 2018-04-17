import QtQuick 2.0
import "../components"

Grid {
    id: root
    property int baseSize: 32
    property var levelPack: null
    property int start: 0

    signal levelSelected(int index)

    Repeater {
        model: root.columns * root.rows

        delegate: Item {
            id: levelDelegate
            width: root.width / root.columns - 1
            height: root.height / root.rows
            visible: engine.levelCount > root.start + index

            property var level: root.levelPack ? root.levelPack.get(root.start + index) : null
            Image {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: levelShape.top
                    bottomMargin: height / 4
                }
                width: root.baseSize
                height: width / 3
                source: levelDelegate.level ? "../graphics/three-stars-" + Math.floor(3 * levelDelegate.level.highscore / levelDelegate.level.rewardPoints) + ".svg" : ""
                sourceSize.width: width
                sourceSize.height: height
            }

            Rectangle {
                id: levelShape
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }
                width: root.baseSize * 3 / 2
                height: root.baseSize
                Image {
                    anchors.fill: parent
                    source: engine.levelPack != null ? (engine.dataDir + engine.levelPack.id + "/level" + (root.start + index + 1) + "/board-preview.png") : ""
                    sourceSize.width: root.baseSize
                    sourceSize.height: root.baseSize
                    fillMode: Image.PreserveAspectCrop
                }
                visible: engine.highestUnlockedLevel >= root.start + index
                //radius: "medium"

                Rectangle {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                        bottomMargin: 4
                    }
                    width: root.baseSize / 2
                    height: width
                    opacity: 0.8
                    color: app.backgroundColor
                    radius: width / 2

                    GameLabel {
                        anchors.centerIn: parent
                        font.pixelSize: parent.height / 2
                        text: root.start + index + 1
                    }
                }
            }

            Rectangle {
                id: lockedLevelShape
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }
                width: root.baseSize * 3 / 2
                height: root.baseSize
                color: "white"
                visible: engine.highestUnlockedLevel < root.start + index
                //radius: "medium"

                Image {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                        bottomMargin: 4
                    }
                    width: root.baseSize / 2
                    height: width
                    source: "../graphics/lock.svg"
                    sourceSize.height: height
                    sourceSize.width: width
                }
            }
            MouseArea {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }
                width: root.baseSize * 3 / 2
                height: root.baseSize
                onClicked: root.levelSelected(root.start + index)
            }
        }
    }
}
