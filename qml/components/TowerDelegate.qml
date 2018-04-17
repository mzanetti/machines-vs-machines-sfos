import QtQuick 2.0
import harbour.machines.vs.machines.sfos.Machines 1.0

Item {
    id: root

    property alias source: animatedDelegate.source
    property alias baseSize: animatedDelegate.baseSize
    property alias spriteCount: animatedDelegate.frameCount
    property alias startFrame: animatedDelegate.startFrame
    property alias duration: animatedDelegate.duration
    property real radius: 0
    property bool showRadius: false
    property alias running: animatedDelegate.running
    property alias onDemand: animatedDelegate.onDemand
    property bool locked: false
    property alias cost: priceTag.price

    function play() {
        animatedDelegate.play();
    }

    function resetToStartFrame() {
        animatedDelegate.resetToStartFrame();
    }

    Rectangle {
        id: radiusRect
        anchors.fill: parent
        anchors.margins: -root.radius * root.width
        color: app.backgroundColor
        border.width: 1
        border.color: "lightgrey"
        opacity: root.showRadius || radiusAnimation.running ? 0.2 : 0
        radius: width / 2 - 1
        Behavior on anchors.margins { NumberAnimation { id: radiusAnimation; duration: 100 } }
    }

    AnimatedDelegate {
        id: animatedDelegate
        anchors.fill: parent
        running: true
        spriteSet: 0
        opacity: root.enabled ? 1 : .3
    }

    Image {
        source: "../graphics/lock.svg"
        anchors { fill: parent; leftMargin: root.width / 2; topMargin: root.height / 2 }
        visible: root.locked
        sourceSize.width: width
        sourceSize.height: height
    }

    PriceTag {
        id: priceTag
        height: root.height * .4
        maxWidth: root.width * 1.5
        visible: !root.locked && root.cost
    }
}
