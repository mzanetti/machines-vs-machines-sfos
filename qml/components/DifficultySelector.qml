import QtQuick 2.0
import QtQuick.Layouts 1.1
import harbour.machines.vs.machines.sfos.Machines 1.0

Item {
    id: root

    property int difficulty: 1

    signal clicked(int difficulty)

    states: [
        State {
            name: "easy"; when: difficulty == Engine.DifficultyEasy
            PropertyChanges { target: bubble; height: priv.baseSize; color: "#27ae60"; anchors.horizontalCenterOffset: priv.baseSize * 1.20 }
            PropertyChanges { target: progressBar; marker1: priv.baseSize * 1.20; marker2: priv.baseSize * 1.20; color: "#27ae60" }
            PropertyChanges { target: label; text: "easy"; horizontalAlignment: Text.AlignLeft; color: "#27ae60" }
        },
        State {
            name: "medium"; when: difficulty == Engine.DifficultyMedium
            PropertyChanges { target: bubble; height: priv.baseSize * 1.6; color: "#e67e22"; anchors.horizontalCenterOffset: priv.baseSize * 6.05 }
            PropertyChanges { target: progressBar; marker1: priv.baseSize * 6.05; marker2: priv.baseSize * 6.05; color: "#e67e22" }
            PropertyChanges { target: label; text: "medium"; horizontalAlignment: Text.AlignHCenter; color: "#e67e22" }
        },
        State {
            name: "hard"; when: difficulty == Engine.DifficultyHard
            PropertyChanges { target: bubble; height: priv.baseSize * 2; color: "#c0392b"; anchors.horizontalCenterOffset: priv.baseSize * 11.4 }
            PropertyChanges { target: progressBar; marker1: priv.baseSize * 11.4; marker2: priv.baseSize * 11.4; color: "#c0392b" }
            PropertyChanges { target: label; text: "hard"; horizontalAlignment: Text.AlignRight; color: "#c0392b" }
        }
    ]
    transitions: [
        Transition {
            from: "*"; to: "*"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: label; property: "opacity"; to: 0; duration: 333; }
                    NumberAnimation { target: bubble; property: "height"; to: priv.baseSize * 0.5; duration: 333; }
                    NumberAnimation { target: progressBar; property: "marker1"; duration: 333 }
                    ColorAnimation { targets: [progressBar,bubble]; property: "color"; duration: 333}
                }
                PropertyAction { target: bubble; property: "visible"; value: false }
                ParallelAnimation {
                    PropertyAction { target: label; properties: "horizontalAlignment,color,text" }
                    PropertyAction { target: bubble; properties: "anchors.horizontalCenterOffset" }
                }
                PropertyAction { target: bubble; property: "visible"; value: true }
                ParallelAnimation {
                    NumberAnimation { target: label; property: "opacity"; to: 1; duration: 333; }
                    NumberAnimation { target: bubble; property: "height"; from: priv.baseSize * 0.5; duration: 333; }
                    NumberAnimation { target: progressBar; property: "marker2"; duration: 333; }
                }
            }
        }
    ]

    QtObject {
        id: priv
        property real baseSize: root.width / 13

        property real progress: (root.difficulty - 1) / 2
        Behavior on progress {
            NumberAnimation {}
        }
    }

    Image {
        id: image
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }
        height: width / 3.5
        source: "../graphics/difficulty-selector.svg"
        sourceSize.width: root.width
        sourceSize.height: root.height

        Rectangle {
            id: bubble
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.left
                horizontalCenterOffset: priv.baseSize * 1.2
            }
            height: priv.baseSize * 0.5
            width: height
            radius: height / 2
        }

        Rectangle {
            id: progressBar
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: (marker1 < marker2 ? marker1 : marker2) - priv.baseSize * 0.25
                right: parent.right
                rightMargin: root.width - ((marker1 > marker2 ? marker1 : marker2) + priv.baseSize * 0.25)
            }
            height: priv.baseSize * 0.5
            radius: height / 2

            property int marker1: priv.baseSize * 1.25
            property int marker2: priv.baseSize * 1.25
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                print(3 * mouseX / width)
                root.clicked(Math.floor(3 * mouseX / width))
            }
        }
    }

    GameLabel {
        id: label
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            rightMargin: height / 4
        }
        font.pixelSize: root.height / 4
    }
}
