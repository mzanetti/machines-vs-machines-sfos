import QtQuick 2.0

MouseArea {
    id: root
    implicitWidth: Math.max(app.buttonSize * 3, label.implicitWidth + app.margins * 2)
    height: app.buttonSize

    property color color: shape.color

    property alias text: label.text
    property alias textColor: label.color


    Rectangle {
        id: shape
        anchors.fill: parent
        radius: 5
        color: root.pressed ?
                   Qt.rgba(root.color.r - .1, root.color.g - .1, root.color.b - .1, root.color.a)
                 : root.color
        Behavior on color { ColorAnimation { duration: 100 } }
    }
    GameLabel {
        id: label
        anchors.centerIn: parent
        font.pixelSize: root.height / 2
        color: app.buttonTextColor
    }
}
