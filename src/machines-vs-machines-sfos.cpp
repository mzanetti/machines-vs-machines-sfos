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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QLocale>
#include <QScopedPointer>
#include <QStandardPaths>
#include <QQuickView>

#include <sailfishapp.h>

#include "engine.h"
#include "settings.h"
#include "level.h"
#include "field.h"
#include "towerproxymodel.h"
#include "boardproxymodel.h"
#include "levelpacks.h"
#include "levelpack.h"
#include "board.h"
#include "enemy.h"
#include "tower.h"
#include "towerfactory.h"

QObject* createSettings(QQmlEngine *engine, QJSEngine *jsEngine)
{

    return new Settings();
}

void migrateSettings()
{
QDir dir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (dir.exists("harbour-machines-vs-machines-sfos"))
    {
        dir.mkpath("com.github.mzanetti/harbour-machines-vs-machines-sfos");
        if (dir.exists("harbour-machines-vs-machines-sfos/harbour-machines-vs-machines-sfos.conf"))
        {
            dir.rename("harbour-machines-vs-machines-sfos/harbour-machines-vs-machines-sfos.conf", "com.github.mzanetti/harbour-machines-vs-machines-sfos/harbour-machines-vs-machines-sfos.conf");
        }
        dir.rename("harbour-machines-vs-machines-sfos", "com.github.mzanetti/harbour-machines-vs-machines-sfos/old");
    }
}

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).
    migrateSettings();

    qmlRegisterType<Engine>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Engine");
    qmlRegisterUncreatableType<LevelPacks>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "LevelPacks", "Can't create this in QML. Get it from Engine.");
    qmlRegisterUncreatableType<LevelPack>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "LevelPack", "Can't create this in QML. Get it from LevelPacks.");
    qmlRegisterUncreatableType<Level>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Level", "Can't create this in QML. Get it from LevelPack.");
    qmlRegisterUncreatableType<Board>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Board", "There's only one board per engine.");
    qmlRegisterUncreatableType<Enemies>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Enemies", "Cant create this in QML. Get it from Board.");
    qmlRegisterUncreatableType<Enemy>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Enemy", "Cant create this in QML. Get it from Enemies model.");
    qmlRegisterUncreatableType<Field>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Field", "Cant create this in QML. Get it from Board.");
    qmlRegisterUncreatableType<Tower>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "Tower", "Cant create this in QML. Get it from Board.");
    qmlRegisterUncreatableType<TowerFactory>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "TowerFactory", "Cant create this in QML. Get it from Engine.");
    qmlRegisterType<TowerProxyModel>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "TowerProxyModel");
    qmlRegisterType<BoardProxyModel>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "BoardProxyModel");

    //qmlRegisterSingletonType<Settings>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "SettingsBackend", createSettings);

    //return SailfishApp::main(argc, argv);


    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));

    app->setApplicationName("harbour-machines-vs-machines-sfos");
    app->setOrganizationDomain("com.github.mzanetti");
    app->setOrganizationName("com.github.mzanetti");
/*
    QString newConfigDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString configFile = QCoreApplication::applicationName() + ".conf";
    QSettings global(newConfigDir + "/" + configFile, QSettings::IniFormat);
*/
    qmlRegisterSingletonType<Settings>("harbour.machines.vs.machines.sfos.Machines", 1, 0, "SettingsBackend", createSettings);

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    view->setSource(SailfishApp::pathTo("qml/harbour-machines-vs-machines-sfos.qml"));
    view->setTitle("Machines vs. Machines");
    view->showFullScreen();

    return app->exec();

}
