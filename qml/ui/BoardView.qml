import QtQuick 2.0
import QtQuick.Layouts 1.1
import harbour.machines.vs.machines.sfos.Machines 1.0
import "../components"
import QtMultimedia 5.0

Item {
    id: root

    property int fieldWidth: root.width / board.columns
    property int fieldHeight: root.height / board.rows

    property var engine
    readonly property variant board: engine.board

    property int selectedField: -1
    property bool fieldOverlay: false

    Image {
        id: backgroundImage
        source: engine.levelPack != null ? engine.dataDir + engine.levelPack.id + "/level" + (engine.level + 1) + "/board.png" : ""
        anchors.centerIn: parent
        height: parent.height + (root.fieldHeight * 4)
        width: parent.width + (root.fieldWidth * 8)
        scale: 0.75
    }
    Component.onCompleted: zoomInAnimation.start()
    SequentialAnimation {
        id: zoomInAnimation
        PauseAnimation { duration: 300 }
        NumberAnimation {
            target: backgroundImage
            property: "scale"
            to: 1
            duration: 4000
            easing.type: Easing.OutQuad
        }
    }


    Repeater {
        id: fieldRepeater
        model: board

        delegate: Item {
            id: fieldDelegate
            x: model.x * width
            y: model.y * height
            width: root.width / board.columns
            height: root.height / board.rows

            Image {
                anchors.fill: parent
                sourceSize.width: width
                sourceSize.height: height
                source: "../graphics/selection-rectangle.svg"
                opacity: root.selectedField == index ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 165 } }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (board.field(index).towerAllowed && !board.field(index).isOnPath) {
                        root.selectedField = index;
                    } else {
                        root.selectedField = -1;
                    }
                }
            }
            Rectangle {
                visible: root.fieldOverlay
                color: "transparent"
                border.color: "black"
                border.width: 3
                anchors.fill: parent
                anchors.margins: 1
                GameLabel {
                    anchors.centerIn: parent
                    text: index
                }
            }
        }
    }

    Repeater {
        id: towerRepeater
        model: BoardProxyModel {
            id: towerModel
            sourceModel: board
        }

        delegate: TowerDelegate {
            x: model.x * width
            y: model.y * height
            width: root.fieldWidth
            height: root.fieldHeight
            id: towerDelegate
            property var tower: towerModel.field(index) ? towerModel.field(index).tower : null
            source: tower ? engine.dataDir + tower.image : ""
            baseSize: tower ? tower.imageBaseSize: 0
            spriteCount: tower ? tower.spriteCount : 0
            startFrame: tower ? tower.startFrame: 0
            duration: tower ? tower.animationDuration : 1000
            radius: tower ? tower.radius : 0
            running: engine.playStatus === Engine.PlayStatusRunning
            onDemand: tower ? (tower.shotType == Tower.ShotTypeCharge || tower.shotType == Tower.ShotTypeRadial) : false
            showRadius: board.field(root.selectedField) && board.field(root.selectedField).tower == tower

            function playShotSound() {
                if (SettingsBackend.audioEnabled) {
                    shotAudio.stop();
                    shotAudio.play();
                }
            }

            SoundEffect {
                id: shotAudio
                source: tower ? engine.dataDir + tower.shotSound + ".wav" : ""
                //audioRole: QMediaPlayer.alert
                volume: 1.0 * SettingsBackend.effectsVolume / 100
            }
        }
    }

    Connections {
        target: root.engine ? root.engine : null

        onPlayStatusChanged: {
            if (engine.playStatus === Engine.PlayStatusNotStarted
                    || engine.playStatus === Engine.PlayStatusLost
                    || engine.playStatus === Engine.PlayStatusWon) {
                root.selectedField = -1
            }
        }

        onTick: {
            for (var i = 0; i < towerRepeater.count; i++) {
                var field = towerModel.field(i);
                var tower = field.tower;
                if (!tower || !tower.canShoot || tower.charging) {
                    continue;
                }
//                print("ticking away")

                var towerDelegate = towerRepeater.itemAt(i)
                var towerPoint = towerDelegate.mapToItem(root, towerDelegate.width / 2, towerDelegate.height / 2);

                // Checking for collisions
                var inRangeEnemies = new Array()
                for (var j = 0; j < enemyRepeater.count; j++) {
                    var enemy = board.enemies.get(j);
                    if (enemy.energy <= 0 || !enemy.running || enemy.currentField === Enemy.TravelStatusExit) {
                        continue;
                    }

                    var enemyDelegate = enemyRepeater.itemAt(j)
                    var enemyPoint = enemyDelegate.mapToItem(root, enemyDelegate.width / 2, enemyDelegate.height / 2);

                    if (board.intersects(Qt.point(towerPoint.x, towerPoint.y), field.tower.radius * root.fieldWidth + root.fieldWidth / 2, Qt.point(enemyPoint.x, enemyPoint.y))) {
                        var target = new Object();
                        target.enemy = enemy;
                        target.enemyDelegate = enemyDelegate;
                        insertSorted(inRangeEnemies, target);
                    }
                }

                // For charged, radial towers, paint shots in all directions, regardless if there's still an enemy around
                if (tower.shotType === Tower.ShotTypeRadial && tower.charged) {
                    towerDelegate.playShotSound();
                    var shotList = prepareRadialShot(tower, towerDelegate);
                    for (var j = 0; j < shotList.length; j++) {
                        shoot(shotList[j]);
                    }
                    towerDelegate.resetToStartFrame();
                }

                if (inRangeEnemies.length == 0) {
                    // Nothing to shoot...
                    // For charged, radial towers, shoot into the void to trigger recovery/charging logik
                    if (tower.shotType === Tower.ShotTypeRadial && tower.charged) {
                        tower.shoot();
                    }
                    continue;
                }

                // If the tower needs to be charged, do that and return
                if ((tower.shotType == Tower.ShotTypeCharge || tower.shotType == Tower.ShotTypeRadial) && !tower.charged) {
                    tower.charge();
                    towerDelegate.play();
                    continue;
                }
                if (tower.shotType == Tower.ShotTypeCharge) {
                    towerDelegate.resetToStartFrame();
                }

                // Ok, we have something to shoot and are fully charged (or don't require charging)
                if (tower.shotType !== Tower.ShotTypeRadial) { // Radial towers already did this
                    towerDelegate.playShotSound();
                }

                // Now shoot the enemies
                for (var j = 0; j < inRangeEnemies.length && (j < tower.shotCount || tower.shotCount == 0) && !tower.charging; j++) {
                    // Shoot all the things! Update the logic
                    tower.shoot(inRangeEnemies[j].enemy);

                    // If not a radial tower, we need to paint the shot from the tower to the enemy
                    if (tower.shotType !== Tower.ShotTypeRadial) {
                        var shot = prepareShot(tower, towerDelegate, inRangeEnemies[j].enemyDelegate);
                        shoot(shot); // Paint the shot
                    }
                }
            }
        }
    }

    function insertSorted(list, target) {
        var inserted = false;
        for (var i = 0; i < list.length; i++) {
            if (list[i].enemy.travelledDistance() < target.enemy.travelledDistance()) {
                list.splice(i, 0, target);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            list.push(target);
        }
    }

    function prepareShot(tower, towerDelegate, targetDelegate) {
        var properties = new Object();
        properties.image = engine.dataDir + tower.shotImage;
        properties.baseSize = tower.imageBaseSize;
        properties.startX = towerDelegate.mapToItem(root, 0, 0).x
        properties.startY = towerDelegate.mapToItem(root, 0, 0).y
        //properties.startX = towerDelegate.mapToItem(root).x
        //properties.startY = towerDelegate.mapToItem(root).y
        properties.startPoint = tower.shotCenter
        properties.startDistance = tower.shotStartDistance
        properties.duration = tower.shotDuration
        properties.lifetime = tower.shotLifetime
        properties.target = targetDelegate;
        properties.move = tower.shotType !== Tower.ShotTypeBeam;
        properties.animate = tower.shotType == Tower.ShotTypeAnimated
        return properties;
    }

    function prepareRadialShot(tower, towerDelegate) {
        var toBeShotList = new Array();
        var targetLocation = new Object();
        targetLocation.x = towerDelegate.x - root.fieldWidth * tower.radius
        targetLocation.y = towerDelegate.y
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x - root.fieldWidth * tower.radius * 0.75
        targetLocation.y = towerDelegate.y - root.fieldHeight * tower.radius * 0.75
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x
        targetLocation.y = towerDelegate.y - root.fieldHeight * tower.radius
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x + root.fieldWidth * tower.radius * 0.75
        targetLocation.y = towerDelegate.y - root.fieldHeight * tower.radius * 0.75
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x + root.fieldWidth * tower.radius
        targetLocation.y = towerDelegate.y
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x + root.fieldWidth * tower.radius * 0.75
        targetLocation.y = towerDelegate.y + root.fieldHeight * tower.radius * 0.75
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x
        targetLocation.y = towerDelegate.y + root.fieldHeight * tower.radius
        toBeShotList.push(targetLocation);
        targetLocation = new Object();
        targetLocation.x = towerDelegate.x - root.fieldWidth * tower.radius * 0.75
        targetLocation.y = towerDelegate.y + root.fieldHeight * tower.radius * 0.75
        toBeShotList.push(targetLocation);

        var shots = new Array();
        for (var j = 0; j < toBeShotList.length; j++) {
            var properties = new Object();
            properties.image = engine.dataDir + tower.shotImage;
            properties.baseSize = tower.imageBaseSize;
            properties.startX = towerDelegate.x
            properties.startY = towerDelegate.y
            properties.startPoint = tower.shotCenter;
            properties.startDistance = tower.shotStartDistance;
            properties.duration = tower.shotDuration;
            properties.lifetime = tower.shotLifetime;
            properties.targetX = toBeShotList[j].x
            properties.targetY = toBeShotList[j].y
            properties.target = new Object();
            properties.target.z = 1;
            properties.move = true;
            properties.animate = tower.shotType == Tower.ShotTypeAnimated
            shots.push(properties)
        }

        return shots;
    }

    function shoot(properties) {
        var shot = shotComponent.createObject(root, properties);
        shot.z = Qt.binding(function() {
            return properties.target.z + (shot.y >= properties.target.y ? 1 : -1);
        })
    }

    Component {
        id: shotComponent
        ShotBase {
            board: root
        }
    }

    Repeater {
        id: enemyRepeater
        model: board.enemies

        delegate: EnemyDelegate {
            fieldWidth: root.fieldWidth
            fieldHeight: root.fieldHeight
            opacity: 0
            z: 100000 + (model.nextY * 100) + (model.y * 50) + (model.nextY > model.y ? model.travelledDistance : 0)

            onRunningChanged: {
                if (running) {
                    opacity = 1
                } else {
                    opacity = 0;
                }
            }
            Behavior on opacity { NumberAnimation { easing.type: Easing.Linear; duration: model.animationDuration / 2 } }

            SoundEffect {
                id: explodeAudio
                source: Qt.resolvedUrl(engine.dataDir + "/" + engine.levelPack.id + "/enemies/Explosion.wav")
                volume: 1.0 * SettingsBackend.effectsVolume / 100
            }

            Connections {
                target: board.enemies.get(index)
                onDied: {
                    if (SettingsBackend.audioEnabled) {
                        explodeAudio.play()
                    }
                }
            }
        }
    }

/*    UbuntuShapeForItem {
        z: 1000000
        id: towerSelectorShape
        anchors.fill: towerSelector
        image: towerSelector
        opacity: showTowerPopup ? 1 : 0
        //Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SnapDuration}}

    }
*/
    property bool showTowerPopup: root.selectedField > -1
                                  && board.field(root.selectedField).tower !== null
                                  && board.field(root.selectedField).tower.level < board.field(root.selectedField).tower.levels

    Image {
        id: popupArrow
        z: 1000000
        anchors {
            centerIn: towerSelector
            verticalCenterOffset: (towerSelector.height / 2 + height / 2 - 1) * (towerSelector.topOrBottom == "top" ? 1 : -1)
            horizontalCenterOffset: -towerSelector.horizontalCenterOffset
        }
        height: 16
        width: 32
        source: "../graphics/quicklist_tooltip.png"
        rotation: towerSelector.topOrBottom == "top" ? 0 : 180
        opacity: showTowerPopup ? 0.7 : 0
    }

    MouseArea { anchors.fill: parent; enabled: towerSelector.visible ; onClicked: root.selectedField = -1 }
    MouseArea { anchors.fill: towerSelector; enabled: towerSelector.visible } // eat mouse presses behind the popup
    Item {
        id: towerSelector
        z: 1000000
        width: availableTowersGrid.width + 16
        height: availableTowersGrid.height + 16
        visible: showTowerPopup
        anchors.centerIn: selectedFieldDelegate ? selectedFieldDelegate : undefined
        anchors.verticalCenterOffset: topOrBottom == "bottom" ? height + 8 : -height - 8
        anchors.horizontalCenterOffset: horizontalCenterOffset
        property var selectedFieldDelegate: root.selectedField >= 0 ? fieldRepeater.itemAt(root.selectedField) : undefined
        property string topOrBottom: selectedFieldDelegate ? selectedFieldDelegate.y < height + 8 ? "bottom" : "top" : ""
        property int horizontalCenterOffset: selectedFieldDelegate ? selectedFieldDelegate.x + selectedFieldDelegate.width / 2 < width / 2 + 8 ?
                                                                        width / 2 - selectedFieldDelegate.width / 2 + 8 :
                                                                        root.selectedField && selectedFieldDelegate.x + selectedFieldDelegate.width / 2 > root.width - width / 2 - 8 ?
                                                                            -width / 2 + selectedFieldDelegate.width / 2 - 8: 0 : 0

        Rectangle {
            anchors.fill: parent
            color: app.backgroundColor
            opacity: 0.7
        }

        Row {
            id: availableTowersGrid
            anchors.centerIn: parent
            property int cellWidth: root.fieldWidth
            property int cellHeight: root.fieldHeight
            property var baseTower: root.selectedField > -1 ? board.field(root.selectedField).tower : null
            property var baseTowerTemplate: baseTower ? engine.availableTowers.getByTowerId(baseTower.id, baseTower.level) : null
            property var upgradeTowerTemplate: baseTower ? engine.availableTowers.getByTowerId(baseTower.id, baseTower.level + 1) : null

            Image {
                height: availableTowersGrid.cellHeight
                width: availableTowersGrid.cellWidth
                source: "../graphics/tower-info.svg"
                sourceSize.height: height
                sourceSize.width: width
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        towerInfoDialog.lineup = engine.availableTowers.getLineupByTowerId(availableTowersGrid.baseTower.id);
                        towerInfoDialog.selectedLevel = availableTowersGrid.baseTower.level;
                        towerInfoDialog.shown = true;
                    }
                }
            }

            function upgradeTower(baseTower) {
                engine.upgradeTower(baseTower);
                root.selectedField = -1;
            }

            TowerDelegate {
                id: boardTowerDelegate
                width: availableTowersGrid.cellWidth
                height: availableTowersGrid.cellHeight

                property var upgradeTowerTemplate: availableTowersGrid.upgradeTowerTemplate !== null && availableTowersGrid.upgradeTowerTemplate.imageBaseSize >= 0 ? availableTowersGrid.upgradeTowerTemplate : null

                radius: upgradeTowerTemplate ? upgradeTowerTemplate.radius : 0
                source: upgradeTowerTemplate ? engine.dataDir + upgradeTowerTemplate.image : ""
                baseSize: upgradeTowerTemplate ? upgradeTowerTemplate.imageBaseSize : 0
                spriteCount: upgradeTowerTemplate ? upgradeTowerTemplate.spriteCount : 1
                duration: upgradeTowerTemplate ? upgradeTowerTemplate.animationDuration : 0
                running: false
                startFrame: 0
                locked: upgradeTowerTemplate && upgradeTowerTemplate.locked
                showRadius: false
                enabled: upgradeTowerTemplate && engine.money >= upgradeTowerTemplate.cost && !upgradeTowerTemplate.locked
                cost: upgradeTowerTemplate ? upgradeTowerTemplate.cost : 0
                visible: upgradeTowerTemplate !== null

                MouseArea {
                    anchors.fill: parent
                    enabled: parent.upgradeTowerTemplate && engine.money >= parent.upgradeTowerTemplate.cost
                    onClicked: {
                        if (boardTowerDelegate.locked) {
                            towerInfoDialog.lineup = engine.availableTowers.getLineupByTowerId(upgradeTowerTemplate.id);
                            towerInfoDialog.selectedLevel = availableTowersGrid.baseTower.level + 1;
                            towerInfoDialog.shown = true;
                            return;
                        }

                        if (availableTowersGrid.baseTower) {
                            if (availableTowersGrid.baseTower.level + 1 < availableTowersGrid.baseTower.levels) {
                                availableTowersGrid.upgradeTower(availableTowersGrid.baseTower)
                            }
                        } else {
                            engine.addTower(oldSelected, index)
                        }
                    }
                }
            }
            MouseArea {
                width: availableTowersGrid.cellWidth
                height: availableTowersGrid.cellHeight

                onClicked: engine.destroyTower(root.selectedField)

                Image {
                    anchors.fill: parent
                    source: "../graphics/trashcan.svg"
                    sourceSize.height: height
                    sourceSize.width: width
                    z: 0
                    // There seems to be some weird bug in qml where this image gets painted above
                    // the price tag declared below. Enabling clipping (which in turn disables
                    // batched rendering seems to prevent that from happening.
                    clip: true
                }

                PriceTag {
                    price: "+" + (availableTowersGrid.baseTowerTemplate ? availableTowersGrid.baseTowerTemplate.destroyReward : "0")
                    visible: availableTowersGrid.baseTowerTemplate ? !availableTowersGrid.baseTowerTemplate.locked : false
                    maxWidth: parent.width * 1.5
                    height: parent.width *.4
                    z: 5000000000
                }
            }
        }
    }
}
