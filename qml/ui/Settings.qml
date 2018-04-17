import QtQuick 2.0
import QtQuick.Layouts 1.1
import "../components"
import harbour.machines.vs.machines.sfos.Machines 1.0

Overlay {
    id: root
    color: app.backgroundColor
    property bool showDeveloperOptions: false

    Column {
        anchors.fill: parent
        anchors.margins: app.margins
        spacing: app.margins

        GameLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: app.titleSize
            font.bold: true
            text: root.showDeveloperOptions ? "Settings" : "Sound settings"
        }

        Row {
            width: parent.width
            spacing: app.margins
            visible: root.showDeveloperOptions
            Rectangle {
                width: parent.width - soundHeaderLabel.width -  app.margins
                height: 1
                anchors.verticalCenter: parent.verticalCenter
                color: app.textColor
            }
            GameLabel {
                id: soundHeaderLabel
                text: "Sound"
            }
        }

        Grid {
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 2
            columnSpacing: app.margins
            width: parent.width * 0.8

            GameLabel {
                text: "sound enabled"
            }
            MouseArea {
                height: 50
                width: height
                Rectangle {
                    anchors.fill: parent
                    color: SettingsBackend.audioEnabled ? app.confirmationButtonColor : app.cancelButtonColor
                    radius: 8
                    Image { anchors.fill: parent; source: "../graphics/tick.svg"; visible: SettingsBackend.audioEnabled }
                }
                onClicked: SettingsBackend.audioEnabled = !SettingsBackend.audioEnabled
            }
            GameLabel {
                text: "soundtrack volume"
            }

            MouseArea {
                height: 50
                width: parent.width - x
                Rectangle { width: parent.width; height: 10; color: "black"; anchors.verticalCenter: parent.verticalCenter }
                Rectangle { height: parent.height; width: height; radius: 8; x: (parent.width * SettingsBackend.volume / 100) - width / 2 }
                onMouseXChanged: {
                    SettingsBackend.volume = Math.max(0,Math.min(100, mouseX * 100 / width))
                }
            }

/*            Slider {
                width: parent.width - x
                minimumValue: 0
                maximumValue: 100
                value: SettingsBackend.volume
                onValueChanged: SettingsBackend.volume = value
            }
*/
            GameLabel {
                text: "effects volume"
            }

            MouseArea {
                height: 50
                width: parent.width - x
                Rectangle { width: parent.width; height: 10; color: "black"; anchors.verticalCenter: parent.verticalCenter }
                Rectangle { height: parent.height; width: height; radius: 8; x: (parent.width * SettingsBackend.effectsVolume / 100) - width / 2 }
                onMouseXChanged: {
                    SettingsBackend.effectsVolume = Math.max(0,Math.min(100, mouseX * 100 / width))
                }
            }

/*            Slider {
                width: parent.width - x
                value: SettingsBackend.effectsVolume
                onValueChanged: SettingsBackend.effectsVolume = value
            }
*/        }

        Row {
            width: parent.width
            spacing: app.margins
            visible: root.showDeveloperOptions
            Rectangle {
                width: parent.width - develHeaderLabel.width -  app.margins
                height: 1
                anchors.verticalCenter: parent.verticalCenter
                color: app.textColor
            }
            GameLabel {
                id: develHeaderLabel
                text: "Levelpack development"
            }
        }

        GridLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 4
            visible: root.showDeveloperOptions
            width: parent.width * 0.8
            GameLabel {
                text: "field overlay"
            }
            Row {
                Layout.fillWidth: true
                MouseArea {
                    height: 50
                    width: height
                    Rectangle {
                        anchors.fill: parent
                        color: SettingsBackend.fieldOverlayEnabled ? app.confirmationButtonColor : app.cancelButtonColor
                        radius: 8
                        Image { anchors.fill: parent; source: "../graphics/tick.svg"; visible: SettingsBackend.fieldOverlayEnabled }
                    }
                    onClicked: SettingsBackend.fieldOverlayEnabled = !SettingsBackend.fieldOverlayEnabled
                }
            }
            GameLabel {
                text: "all unlocked"
            }
            Row {
                Layout.fillWidth: true
                MouseArea {
                    height: 50
                    width: height
                    Rectangle {
                        anchors.fill: parent
                        radius: 8
                        color: SettingsBackend.allUnlocked ? app.confirmationButtonColor : app.cancelButtonColor
                        Image { anchors.fill: parent; source: "../graphics/tick.svg"; visible: SettingsBackend.allUnlocked }
                    }
                    onClicked: SettingsBackend.allUnlocked = !SettingsBackend.allUnlocked
                }
            }
        }
    }
}
