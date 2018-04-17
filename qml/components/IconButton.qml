import QtQuick 2.0

MouseArea {
    id: root

    property bool hasPressedState: true
    property string iconName
    property string iconSource

    Image {
        id: image
        anchors.fill: parent
        source: root.iconName
                ? "../graphics/" + root.iconName + (root.pressed && root.hasPressedState ? "-pushed" : "") + ".svg"
                : root.iconSource
        sourceSize.width: width
        sourceSize.height: height
    }
}

