import QtQuick 2.0

Item {
    id: root

    width: board.fieldWidth / 2
    height: board.fieldHeight / 2
    x: startX
    y: startY

    property string image
    property int baseSize
    property int duration
    property int lifetime
    property var target: null
    property var board
    property int startDistance
    property int startX
    property int startY
    property var startPoint
    property bool move: false
    property bool animate: false

    property int targetX: target ? target.x + (target.width - root.width) / 2 - xTranslate : startX
    property int targetY: target ? target.y + (target.height - root.width) / 2 - yTranslate : startY
    readonly property int distanceToTarget: Math.sqrt(Math.pow(targetX - x, 2) + Math.pow(targetY - y, 2)) - startDistance

    readonly property int xTranslate: (root.startPoint.x * root.board.fieldWidth / root.baseSize) - (root.width / 2)
    readonly property int yTranslate: (root.startPoint.y * root.board.fieldHeight / root.baseSize) - (root.height / 2)

    property int ticks: 0

    transform: [
        Rotation {
            origin.x: root.width / 2
            origin.y: root.height / 2
            angle: {
                if (root.animate) {
                    return (root.ticks + Math.random() * 30) % 360;
                }
                var angle = Math.atan((root.y - root.targetY) / (root.x - root.targetX)) * 180 / Math.PI;
                if (root.x >= root.targetX) {
                    angle += 180
                }
                return angle;
            }
        },
        Translate {
            x: root.xTranslate
            y: root.yTranslate
        }
    ]

    Row {
        id: beamRow
        width: root.distanceToTarget + root.startDistance + root.width / 2
        clip: !root.move
        Item {
            height: root.height
            width: root.startDistance * root.board.fieldWidth / root.baseSize + (move ? 0 : root.width / 2)
        }

        Repeater {
            model: root.move ? 1 : Math.ceil(root.distanceToTarget / root.width)
            Image {
                id: beamImage
                width: root.width
                height: root.height
                source: root.image
                sourceSize.width: root.width
                sourceSize.height: root.height
            }
        }
    }

    Connections {
        target: engine
        onTick: {
            ticks++;
            var ms = 1000 * ticks / (engine.ticksPerSecond / engine.gameSpeed)
            if (ms >= root.lifetime || root.target === undefined) {
                root.destroy();
            }

            if (!root.move) {
                return;
            }

            var progress = Math.min(ms / root.duration, 1);
            root.x = root.startX + (root.targetX - root.startX) * progress
            root.y = root.startY + (root.targetY - root.startY) * progress
        }
    }
}
