import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.0
import harbour.machines.vs.machines.sfos.Machines 1.0

Item {
    id: root
    width: root.fieldWidth
    height: root.fieldHeight
    x: model.x * root.fieldWidth
    y: model.y * root.fieldHeight

    property int fieldWidth
    property int fieldHeight

    property int currentField: model.currentField
    onCurrentFieldChanged: controller.reload();

    readonly property bool running: engine.playStatus === Engine.PlayStatusRunning && model.running

    AnimatedDelegate {
        id: leftDelegate
        anchors.fill: parent
        visible: model.walkingDirection === Enemy.WalkingDirectionRight
        running: root.running
        source: Qt.resolvedUrl(engine.dataDir +  model.image)
        baseSize: model.imageBaseSize
        frameCount: model.spriteCount
        duration: model.animationDuration
        spriteSet: 0
        startFrame: 0
    }

    AnimatedDelegate {
        id: rightDelegate
        anchors.fill: parent
        visible: model.walkingDirection === Enemy.WalkingDirectionLeft || model.spriteCount <= 1
        running: root.running
        source: Qt.resolvedUrl(engine.dataDir +  model.image)
        baseSize: model.imageBaseSize
        frameCount: model.spriteCount
        duration: model.animationDuration
        spriteSet: 1
        startFrame: 0
    }

    AnimatedDelegate {
        id: frontDelegate
        anchors.fill: parent
        visible: model.walkingDirection === Enemy.WalkingDirectionDown
        running: root.running
        source: Qt.resolvedUrl(engine.dataDir +  model.image)
        baseSize: model.imageBaseSize
        frameCount: model.spriteCount
        duration: model.animationDuration
        spriteSet: 2
        startFrame: 0
    }

    AnimatedDelegate {
        id: backDelegate
        anchors.fill: parent
        visible: model.walkingDirection === Enemy.WalkingDirectionUp
        running: root.running
        source: Qt.resolvedUrl(engine.dataDir +  model.image)
        baseSize: model.imageBaseSize
        frameCount: model.spriteCount
        duration: model.animationDuration
        spriteSet: 3
        startFrame: 0
    }

    AnimationController {
        id: controller
        progress: model.progress

        animation: SequentialAnimation {
            id: advanceAnimation
            ParallelAnimation {
                NumberAnimation { target: root; property: "x"; from: model.x * root.fieldWidth; to: model.nextX * root.fieldWidth }
                NumberAnimation { target: root; property: "y"; from: model.y * root.fieldHeight; to: model.nextY * root.fieldHeight }
            }
        }
    }

    Rectangle {
        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter }
        height: parent.height / 15
        width: parent.width / 2
        color: "lightgray"

        Rectangle {
            anchors { fill: parent; rightMargin: (model.maxEnergy - model.energy) * parent.width / model.maxEnergy }
            color: "orange"
        }
    }

    property bool alive: model.energy > 0
    onAliveChanged: {
        if (!alive) {
            emitter1.pulse(500)
            emitter2.pulse(400)
        }
    }
    ParticleSystem {
        id: sys1
    }

    ParticleSystem {
        id: sys2
    }
    Emitter {
        id: emitter1

        anchors.centerIn: parent
        system: sys1
        emitRate: 20
        lifeSpan: 500
        size: root.fieldWidth / 4
        endSize: size * 2
        enabled: false
        velocity: AngleDirection {
                   angle: 0
                   angleVariation: 360
                   magnitude: root.fieldWidth * 2
                   magnitudeVariation: 50
               }
    }

    Emitter {
        id: emitter2

        anchors.centerIn: parent
        system: sys2
        emitRate: 30
        lifeSpan: 500
        size: root.fieldWidth / 6
        endSize: size * 2
        enabled: false
        velocity: AngleDirection {
                   angle: 0
                   angleVariation: 360
                   magnitude: root.fieldWidth * 2
                   magnitudeVariation: 50
               }
    }

    ImageParticle {
        source: Qt.resolvedUrl(engine.dataDir + engine.levelPack.id + "/enemies/scrap" + Math.floor(Math.random() * 2) + ".png")
        system: sys1
        colorVariation: 1
        rotation: 0
        rotationVariation: 360
        rotationVelocity: -4*360
    }

    ImageParticle {
        source: Qt.resolvedUrl(engine.dataDir + engine.levelPack.id + "/enemies/scrap" + Math.floor(Math.random() * 3 + 2) + ".png")
        system: sys2
        colorVariation: .5
        rotation: 0
        rotationVariation: 360
        rotationVelocity: 180
    }

}
