import QtQuick 2.0


Item {
    id: root
    anchors.fill: parent
    property alias color: background.color
    property bool showHomeButton: true
    property bool hideBackground: false
    property bool autoClose: true

    signal closeButtonClicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.margins: -app.margins
        color: app.backgroundColor
        border.width: app.margins
        border.color: app.foregroundColor
        visible: !root.hideBackground
    }

    IconButton {
        id: homeButton
        anchors { left: parent.left; bottom: parent.bottom; margins: app.margins }
        height: app.buttonSize
        width: app.buttonSize
        iconName: "main-menu-button"
        hasPressedState: false
        visible: root.showHomeButton
        onClicked: {
            root.closeButtonClicked()
        }
    }
}
