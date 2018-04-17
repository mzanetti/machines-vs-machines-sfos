import QtQuick 2.0

Item {
    id: root
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property string currentLevelPackId: listView.currentIndex > -1 ? listView.itemAt(listView.currentIndex).levelPackId : "no-item"

    Repeater {
        id: listView
        property int currentIndex: -1
        onCountChanged: root.currentLevelPackId = Qt.binding(function() { return listView.itemAt(listView.currentIndex).levelPackId;})

        delegate: Item {
            id: delegate
            height: root.height
            width: height * 2
            anchors.centerIn: parent
            state: ""
            z: 1
            scale: 0.8
            opacity: 0

            property string levelPackId: model.id

            states: [
                State {
                    name: "current"; when: index == listView.currentIndex
                    PropertyChanges { target: delegate; z: 200; scale: 1; opacity: 1 }
                    PropertyChanges { target: overlay; opacity: 0 }
                },
                State {
                    name: "previous"; when: index + 1 == listView.currentIndex
                    PropertyChanges { target: delegate; anchors.horizontalCenterOffset: -root.width / 4; z: 100; opacity: 1 }
                },
                State {
                    name: "next"; when: index - 1 == listView.currentIndex
                    PropertyChanges { target: delegate; anchors.horizontalCenterOffset: root.width / 4; z: 100; opacity: 1 }
                }
            ]
            BorderImage {
                anchors.fill: parent
                anchors.margins: -20
                opacity: 1
                source: "../graphics/dropshadow2gu.sci"
            }

            Rectangle {
                anchors.fill: parent
                color: "white"
            }

            Image {
                anchors.fill: parent
                anchors.margins: parent.height / 20
                source: "../../data/" + model.id + "/" + model.titleImage
                sourceSize.height: height
                sourceSize.width: width
            }

            transitions: [
                Transition {
                    from: "next"; to: "current"
                    ParallelAnimation {
                        PropertyAnimation { properties: "anchors.horizontalCenterOffset"; easing.type: Easing.InBack; easing.overshoot: 7.5; duration: 500 }
                        PropertyAnimation { properties: "scale,opacity,z"; duration: 500 }
                    }
                },
                Transition {
                    from: "previous"; to: "current"
                    ParallelAnimation {
                        PropertyAnimation { properties: "anchors.horizontalCenterOffset"; easing.type: Easing.InBack; easing.overshoot: 7.5; duration: 500 }
                        PropertyAnimation { properties: "scale,opacity,z"; duration: 500 }
                    }
                },
                Transition {
                    from: "current"
                    ParallelAnimation {
                        PropertyAnimation { properties: "anchors.horizontalCenterOffset"; easing.type: Easing.OutBack; easing.overshoot: 7.5; duration: 500 }
                        PropertyAnimation { properties: "scale,opacity,z"; duration: 500 }
                    }
                },
                Transition {
                    ParallelAnimation {
                        PropertyAnimation { properties: "anchors.horizontalCenterOffset,opacity,z"; duration: 500 }
                    }
                }

            ]

            MouseArea {
                anchors.fill: parent
                onClicked: listView.currentIndex = index
            }

            Rectangle {
                id: overlay
                anchors.fill: parent
                color: "white"

                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: height / 4
                    }
                    height: parent.height / 4
                    width: height
                    //color: "black"
                    source: "../graphics/go-previous.svg"
                }
                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        rightMargin: height / 4
                    }
                    height: parent.height / 4
                    width: height
                    //color: "black"
                    source: "../graphics/go-next.svg"
                }
            }
        }
    }
}
