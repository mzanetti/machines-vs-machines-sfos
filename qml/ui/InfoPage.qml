import QtQuick 2.0
import QtQuick.Layouts 1.1
import harbour.machines.vs.machines.sfos.Machines 1.0
import "../components"
import "info"

Overlay {
    id: root
    color: app.backgroundColor

    Row {
        anchors {
            top: parent.top
            topMargin: app.margins
            horizontalCenter: parent.horizontalCenter
        }
        spacing: app.margins

        Image {
            height: parent.height
            width: height
            source: "../graphics/mvm-game-icon.svg"
        }

        Column {
            GameLabel {
                id: titleLabel
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: app.titleSize
                text: "Machines vs Machines"
                font.weight: Font.DemiBold
            }

            GameLabel {
                text: qsTr("Version: 1.3")
                font.pixelSize: app.titleSize * 0.8
            }
        }
    }
    Item {
        anchors {
            fill: parent
            leftMargin: app.buttonSize * 3
            topMargin: app.buttonSize * 3
            rightMargin: app.buttonSize * 3
            bottomMargin: app.buttonSize * 3
        }

        Grid {
            id: infoView
            anchors.centerIn: parent
            columns: 2

            property int maxCellWidth: parent.width / 2
            property int maxCellHeight: parent.height / 2
            property bool scaleToWidth: maxCellWidth > maxCellHeight * 3/2

            Repeater {
                model: ListModel {
                    id: infoModel
//                            ListElement { title: "How to play"; imageSource: ""; target: "howto" }
                    ListElement { title: "Credits"; imageSource: "credits"; target: "credits" }
                    ListElement { title: "License"; imageSource: "license"; target: "license" }
//                            ListElement { title: "Create a level pack"; imageSource: ""; target: "create" }
                    ListElement { title: "Feedback"; imageSource: "feedback"; target: "feedback" }
                    ListElement { title: "Donate"; imageSource: "heart-donation"; target: "donate" }
                }

                MouseArea {
                    width: infoView.scaleToWidth ? infoView.maxCellWidth : infoView.maxCellHeight * 3/2
                    height: infoView.scaleToWidth ? infoView.maxCellWidth * 2 / 3 : infoView.maxCellHeight

                    Rectangle {
                        id: shape
                        anchors.centerIn: parent
                        width: parent.width * .6
                        height: Math.min( parent.height * 0.6, parent.height - itemLabel.height * 2 )
                        //radius: "medium"
                        color: "white"

                        Image {
                            anchors.centerIn: parent
                            source: "../graphics/" + model.imageSource + ".svg"
                            width: shape.height
                            height: shape.height
                            sourceSize.width: shape.height
                            sourceSize.height: shape.height
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    GameLabel {
                        id: itemLabel
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: model.title
                    }
                    onClicked: {
                        switch (model.target) {
                        case "howto": howToPlay.shown = true; break;
                        case "credits": credits.shown = true; break;
                        case "license": license.shown = true; break;
                        case "create": createLevelpack.shown = true; break;
                        case "feedback": feedback.shown = true; break;
                        case "donate": donate.shown = true; break;
                        }
                    }
                }
            }
        }
    }

    PageLoader {
        id: credits
        page: "info/Credits.qml"
    }

    PageLoader {
        id: license
        page: "info/License.qml"
    }

    PageLoader {
        id: donate
        page: "info/Donate.qml"
    }

    PageLoader {
        id: feedback
        page: "info/Feedback.qml"
    }
}
