/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.machines.vs.machines.sfos.Machines 1.0

import "ui"
import "components"

ApplicationWindow
{
    id: app

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.Landscape | Orientation.LandscapeInverted
    _defaultPageOrientations: Orientation.All

    property bool performanceOverlayEnabled: false

    property bool managingTowers: false

    /******************************************************
     *           Global definitions                       *
     ******************************************************/
    // Colors
    property color backgroundColor: "#ededed"
    property color foregroundColor: "#333333"
    property color textColor: foregroundColor
    property color playButtonColor: "#2eacd2"
    property color buttonTextColor: "#ffffff"
    property color confirmationButtonColor: "#35af44"
    property color rejectButtonColor: "#db3131"
    property color cancelButtonColor: "#8f8f8f"
    property string modalWindowBackgroundColor: "#CC000000"

    // Sizes
    property int appHeight: Math.min(app.height, app.width)
    property int buttonSize: appHeight / 10
    property int margins: appHeight / 30
    property int titleSize: appHeight / 25
    property int fontSize: appHeight / 26
    property int hugeFontSize: appHeight / 10

    Component.onCompleted: {
        app.updateAudio();
    }

    Engine {
        id: engine
        dataDir: Qt.resolvedUrl("../data/")
    }

    Audio {
        id: menuMusic
        loops: Audio.Infinite
        volume: 1.0 * SettingsBackend.volume / 100
    }
    Audio {
        id: gamePlayMusic
        loops: Audio.Infinite
        source: engine.levelPack ? Qt.resolvedUrl(engine.dataDir + "/" + engine.levelPack.id + "/sounds/" + engine.levelPack.get(engine.level).soundFile + ".ogg") : ""
        volume: 1.0 * SettingsBackend.volume / 100
    }
/*    ScreenSaver {
        screenSaverEnabled: engine.playStatus !== Engine.PlayStatusRunning
    }
*/
    Connections {
        target: engine
        onPlayStatusChanged: {
            app.updateAudio();
        }
    }
    Connections {
        target: Qt.application
        onActiveChanged: {
            app.updateAudio();
            if (!Qt.application.active && engine.playStatus == Engine.PlayStatusRunning) {
                engine.pauseGame();
            }
        }
    }

    function updateAudio() {
        if (!SettingsBackend.audioEnabled) {
            menuMusic.stop();
            gamePlayMusic.stop();
            return;
        }

        if (!Qt.application.active) {
            menuMusic.pause();
            gamePlayMusic.pause();
            return;
        }

        switch (engine.playStatus) {
        case Engine.PlayStatusNotStarted:
//                menuMusic.stop();
            if (mainPage.item) {
                menuMusic.source = Qt.resolvedUrl(engine.dataDir + "/" + mainPage.item.currentLevelpackId + "/sounds/" + engine.levelPacks.levelPack(mainPage.item.currentLevelpackId).titleSound + ".ogg");
            } else {
                menuMusic.source = Qt.resolvedUrl(engine.dataDir + "/" + engine.levelPack.id + "/sounds/" + engine.levelPacks.levelPack(engine.levelPack.id).titleSound + ".ogg");
            }
            menuMusic.play();
            gamePlayMusic.stop();
            break;
        case Engine.PlayStatusRunning:
        case Engine.PlayStatusPaused:
            menuMusic.stop();
            gamePlayMusic.play();
            break;
        default:
        }
    }

    Connections {
        target: SettingsBackend
        onAudioEnabledChanged: {
            app.updateAudio()
        }
    }

    Rectangle {
        width: app.height
        height: app.width
        anchors.centerIn: parent
        color: "black"
        rotation: app.orientation == Orientation.Landscape ? 90 : -90
        Behavior on rotation {
            NumberAnimation {}
        }
        PageLoader {
            id: mainPage
            page: "MainPage.qml"
            shown: engine.levelPack == null
/*            Binding {
                target: mainPage.item
                property: "showDeveloperOptions"
                value: args.values.d
            }
*/
            Connections {
                target: mainPage.item
                onOpenSettings: settings.shown = true
                onLoadLevelPack: {
                    engine.loadLevelPack(levelPack);
                }
                onOpenInfoPage: {
                    infoPage.shown = true
                }
                onCurrentLevelpackIdChanged: app.updateAudio()
            }
        }

        PageLoader {
            id: settings
            page: "Settings.qml"
/*            Binding {
                target: settings.item
                property: "showDeveloperOptions"
                value: args.values.d
            }
*/
        }

        PageLoader {
            id: levelSelector
            page: "LevelSelector.qml"
            shown: engine.levelPack && engine.playStatus != Engine.PlayStatusRunning && engine.playStatus != Engine.PlayStatusPaused
            Binding {
                target: levelSelector.item
                property: "engine"
                value: engine
            }

            Connections {
                target: levelSelector.item
                onCloseButtonClicked: {
                    engine.unloadLevelPack()
                }
                onLevelSelected: {
                    if (index <= engine.highestUnlockedLevel) {
                        levelSplash.level = index
                        levelSplash.shown = true;
                    }
                }
                onShowScoreInfo: {
                    scoreInfo.shown = true;
                }
            }

        }

        PageLoader {
            id: levelSplash
            page: "LevelSplash.qml"
            property int level: -1
            Binding {
                target:levelSplash.item
                property: "level"
                value: levelSplash.level
            }

            Connections {
                target: levelSplash.item

                onStartLevel: {
                    if(!SettingsBackend.tutorialShown) {
                        tutorial.shown = true;
                    } else {
                        engine.startNewGame(levelSplash.level)
                    }
                }
            }
        }

        PageLoader {
            id: tutorial
            page: "Tutorial.qml"

            Connections {
                target: tutorial.item
                onCloseButtonClicked: {
                    engine.startNewGame(levelSplash.level)
                    SettingsBackend.tutorialShown = true;
                }
            }
        }

        PageLoader {
            id: gameView
            page: "GameView.qml"
            shown: engine.playStatus !== Engine.PlayStatusNotStarted
            visible: shown
            Binding {
                target: gameView.item
                property: "engine"
                value: engine
            }
            Binding {
                target: gameView.item
                property: "fieldOverlay"
                value: SettingsBackend.fieldOverlayEnabled
            }
        }

        PageLoader {
            id: levelResultsSplash
            page: "LevelResultsSplash.qml"
            shown: engine.playStatus == Engine.PlayStatusLost || engine.playStatus == Engine.PlayStatusWon
            Binding {
                target: levelResultsSplash.item
                property: "engine"
                value: engine
            }

            Connections {
                target: levelResultsSplash.item
                onRetry:{
                    engine.startNewGame(engine.level)
                }
            }
        }

        PageLoader {
            id: levelPausedSplash
            page: "LevelPausedSplash.qml"
            shown: engine.playStatus == Engine.PlayStatusPaused
            cached: engine.playStatus == Engine.PlayStatusPaused || engine.playStatus == Engine.PlayStatusRunning
            Binding {
                target: levelPausedSplash.item
                property: "level"
                value: engine.level
            }
        }

        PageLoader {
            id: towerInfoDialog
            page: "UnlockTowerOverlay.qml"
            cached: engine.playStatus == Engine.PlayStatusRunning || engine.playStatus == Engine.PlayStatusPaused

            property var lineup: null
            property int selectedLevel: 0

            Binding {
                target: towerInfoDialog.item
                property: "lineup"
                value: towerInfoDialog.lineup
            }

            onShownChanged: {
                if (shown) {
                    engine.pauseGame();
                    towerInfoDialog.item.selectedLevel = towerInfoDialog.selectedLevel;
                } else {
                    engine.resumeGame();
                }
            }
        }

        PageLoader {
            id: scoreInfo
            page: "ScoreInfo.qml"
        }

        PageLoader {
            id: infoPage
            page: "InfoPage.qml"
        }
    }


/*    Rectangle {
        id: rotateRect
        anchors.fill: parent
        color: app.backgroundColor

        property bool landscape: width > height

        opacity: landscape ? 0 : 1
        Behavior on opacity {
            NumberAnimation {}
        }

        onLandscapeChanged: {
            if (!landscape && engine.playStatus == Engine.PlayStatusRunning) {
                engine.pauseGame();
            }
        }

        GameLabel {
            anchors.centerIn: parent
            text: "Please rotate your device"
            font.pixelSize: app.buttonSize
            rotation: 90
        }
    }
    */
}

