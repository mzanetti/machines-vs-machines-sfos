import QtQuick 2.0
import harbour.machines.vs.machines.sfos.Machines 1.0

Item {
    id: root
    anchors.fill: parent

    property alias source: sprite.source
    property int duration: 1000
    property int baseSize: 256
    property alias frameCount: sprite.frameCount
    property int startFrame: 0
    property int spriteSet: 0
    property bool useEngineClock: true
    property bool running: false
    property bool onDemand: false

    function play() {
        sprite.remainingFrames = sprite.frameCount;
        sprite.remainingTicks = sprite.ticksPerFrame;
    }

    function resetToStartFrame() {
        sprite.currentFrame = root.startFrame;
    }

    AnimatedSprite {
        id: sprite
        anchors.fill: parent
        frameHeight: root.baseSize
        frameWidth: root.baseSize
        frameY: root.spriteSet * root.baseSize
        running: root.running || (root.useEngineClock && engine.playStatus == Engine.PlayStatusPlaying) || root.onDemand
        frameDuration: root.duration / root.frameCount
        paused: root.useEngineClock || root.onDemand
        currentFrame: root.startFrame

        property int msPerFrame: root.duration / root.frameCount
        // msPF : 1000 = tpf : tps
        property int ticksPerFrame: msPerFrame * engine.ticksPerSecond / 1000
        property int remainingTicks: ticksPerFrame

        property int remainingFrames: 0
    }

    Connections {
        target: root.useEngineClock && root.running ? engine : null

        onTick: {
            if (!root.useEngineClock || !root.running) {
                return;
            }
            if (root.onDemand && sprite.remainingFrames <= 0) {
                return;
            }

            sprite.remainingTicks--;
            if(sprite.remainingTicks <= 0 && root.frameCount > 1) {
                if (!root.onDemand || sprite.remainingFrames > 1 ) {
                    sprite.advance();
                }
                sprite.remainingTicks = sprite.ticksPerFrame;

                if (root.onDemand) {
                    sprite.remainingFrames--;
                }
            }
        }
    }
}
