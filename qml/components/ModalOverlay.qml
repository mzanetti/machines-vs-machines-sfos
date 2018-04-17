import QtQuick 2.0
import "../components"

Overlay {
    id: root
    color: app.modalWindowBackgroundColor
    property real contentMargins: 0.14
    property int level: -1
    property string title
    property bool showCloseButton: false
    showHomeButton: false

    default property alias data: contentItem.data

    signal backgroundClicked()

    MouseArea {
        anchors.fill: parent
        onClicked: root.backgroundClicked();
    }

    Rectangle {
        id: innerRect
        anchors {
            fill: parent
            topMargin: root.height * root.contentMargins
            bottomMargin: root.height * root.contentMargins
            leftMargin: root.width * root.contentMargins
            rightMargin: root.width * root.contentMargins
        }

        color: app.backgroundColor

        MouseArea {
            anchors.fill: parent
        }

        GameLabel {
            id: titleLabel
            anchors {
                top: parent.top
                topMargin: app.margins
                horizontalCenter: parent.horizontalCenter
            }
            text: root.title
            visible: root.title
            color: app.textColor
        }

        Item {
            id: contentItem
            anchors {
                fill: parent
                topMargin: root.title ? titleLabel.height + app.margins : 0
                bottomMargin: root.showCloseButton ? closeButton.height + app.margins : 0
            }
        }

        PushButton {
            id: closeButton
            anchors {
                right: parent.right;
                bottom: parent.bottom;
                bottomMargin: app.margins
                rightMargin: app.margins
            }
            width: app.buttonSize * 3
            height: app.buttonSize
            visible: root.showCloseButton
            color: app.cancelButtonColor
            text: qsTr("Close")
            z: 3
            onClicked: {
                root.closeButtonClicked()
            }
        }
    }
}
