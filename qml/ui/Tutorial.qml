import QtQuick 2.0
import QtQuick.Layouts 1.1
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0

Overlay {
    id: root
    color: app.backgroundColor

    signal done()

    Image {
        id: image
        source: "../graphics/tutorial-" + page + ".svg"
        width: parent.width * 2
        height: width / 2
        sourceSize.width: width
        sourceSize.height: height
        anchors.centerIn: parent

        property int page: 1

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (image.page < 6) {
                    image.page++
                } else {
                    print("emitting closed")
                    root.closeButtonClicked()
                }
            }
        }
    }
}
