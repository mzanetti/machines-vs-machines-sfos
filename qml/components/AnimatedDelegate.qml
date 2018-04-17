import QtQuick 2.0
import harbour.machines.vs.machines.sfos.Machines 1.0

Item {
    id: root

    property string source
    property int baseSize: -1
    property int frameCount: -1
    property int startFrame: -1
    property real duration: 1000
    property int spriteSet: -1
    property bool running: false
    property bool useEngineClock: true
    property bool onDemand: false

    function play() {
        spriteLoader.item.play()
    }

    function resetToStartFrame() {
        spriteLoader.item.resetToStartFrame();
    }

    onSourceChanged: loadSprite();
    onBaseSizeChanged: loadSprite();
    onFrameCountChanged: loadSprite();
    onSpriteSetChanged: loadSprite();
    onStartFrameChanged: loadSprite();

    function loadSprite() {
        spriteLoader.source = "";

        if (!source || baseSize == -1 || frameCount == -1 || spriteSet == -1 || startFrame == -1 ) {
            return;
        }
//        print("loading sprite", root.source, root)

        spriteLoader.setSource("UDSpriteSequence.qml",
                               {
                                   source: root.source,
                                   baseSize: root.baseSize,
                                   frameCount: root.frameCount,
                                   spriteSet: root.spriteSet,
                                   startFrame: root.startFrame
                               }
                              );
    }


    Loader {
        id: spriteLoader
        anchors.fill: parent
        asynchronous: false
    }

    Binding {
        target: spriteLoader.item
        property: "running"
        value: root.running && root.frameCount > 0 && root.visible
    }
    Binding {
        target: spriteLoader.item
        property: "useEngineClock"
        value: root.useEngineClock
    }
    Binding {
        target: spriteLoader.item
        property: "duration"
        value: root.duration
    }
    Binding {
        target: spriteLoader.item
        property: "onDemand"
        value: root.onDemand
    }
}
