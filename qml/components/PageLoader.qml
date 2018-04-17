import QtQuick 2.0

Item {
    id: root
    anchors.fill: parent

    property string page: ""
    property bool shown: false
    property alias item: loader.item
    property bool cached: false

    onShownChanged: {
        if (shown) {
            loader.isVisible = true
        }
    }

    Loader {
        id: loader
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        anchors.verticalCenterOffset: root.shown ? 0 : hiddenOffset
        active: root.shown || isVisible || root.cached
        source: "../ui/" + root.page
        visible: root.shown || isVisible


        property bool isVisible
        property int hiddenOffset: -height - app.margins

        Behavior on anchors.verticalCenterOffset {
            SequentialAnimation {
                NumberAnimation { }
                ScriptAction {
                    script: {
                        if (!root.shown) loader.isVisible = false
                    }
                }
            }
        }

        onStatusChanged: print("Loader", root.page, "status changed:", status == Loader.Ready ? "Ready" : status)
    }

    Connections {
        target: loader.item
        ignoreUnknownSignals: true
        onCloseButtonClicked: {
            if (loader.item.autoClose) {
                root.shown = false
            }
        }
        onBackgroundClicked: {
            if (loader.item.autoClose) {
                root.shown = false
            }
        }
    }
}

