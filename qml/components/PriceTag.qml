import QtQuick 2.0

Rectangle {
    anchors { right: parent.right; bottom: parent.bottom }
    width: Math.min(parent.width, Math.max(height, countLabel.implicitWidth + height/2))
    height: 10
    color: "#aa000000"
    border.width: 2
    border.color: "white"
    radius: 10

    property string price
    property int maxWidth: 5*8

    GameLabel {
        id: countLabel
        text: parent.price + "$"
        anchors.centerIn: parent
        width: parent.maxWidth - parent.height
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        color: "white"
    }
}
