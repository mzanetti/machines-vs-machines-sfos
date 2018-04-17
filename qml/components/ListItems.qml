import QtQuick 2.0

MouseArea {
    implicitWidth: row.implicitWidth + app.margins * 2

    property alias iconSource: icon.source
    property alias text: textLabel.text
    property alias layoutDirection: row.layoutDirection

    Row {
        id: row
        anchors {
            top: parent.top
            topMargin: app.margins / 2
            bottom: parent.bottom
            bottomMargin: app.margins / 2
            left: parent.left
            leftMargin: app.margins
        }
        spacing: height / 4

        Rectangle {
            anchors { top: parent.top; bottom: parent.bottom }
            width: height
            color: "#bababa"
            //radius: "medium"

            Image {
                id: icon
                anchors { fill: parent; margins: parent.height / 8 }
            }
        }

        GameLabel {
            id: textLabel
            font.pixelSize: app.fontSize
            anchors.verticalCenter: parent.verticalCenter
            color:  app.textColor
        }
    }
}
