#include <QtQml>
#include <QtQml/QQmlContext>
#include "backend.h"
#include "engine.h"
#include "towerproxymodel.h"
#include "levelpacks.h"
#include "levelpack.h"
#include "level.h"
#include "enemy.h"
#include "tower.h"
#include "towerfactory.h"
#include "board.h"
#include "field.h"
#include "imageprovider.h"
#include "boardproxymodel.h"
#include "settings.h"

QObject* createSettings(QQmlEngine *engine, QJSEngine *jsEngine)
{
    return new Settings();
}

void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Machines"));

    qmlRegisterType<Engine>("Machines", 1, 0, "Engine");
    qmlRegisterUncreatableType<LevelPacks>("Machines", 1, 0, "LevelPacks", "Can't create this in QML. Get it from Engine.");
    qmlRegisterUncreatableType<LevelPack>("Machines", 1, 0, "LevelPack", "Can't create this in QML. Get it from LevelPacks.");
    qmlRegisterUncreatableType<Level>("Machines", 1, 0, "Level", "Can't create this in QML. Get it from LevelPack.");
    qmlRegisterUncreatableType<Board>("Machines", 1, 0, "Board", "There's only one board per engine.");
    qmlRegisterUncreatableType<Enemies>("Machines", 1, 0, "Enemies", "Cant create this in QML. Get it from Board.");
    qmlRegisterUncreatableType<Enemy>("Machines", 1, 0, "Enemy", "Cant create this in QML. Get it from Enemies model.");
    qmlRegisterUncreatableType<Field>("Machines", 1, 0, "Field", "Cant create this in QML. Get it from Board.");
    qmlRegisterUncreatableType<Tower>("Machines", 1, 0, "Tower", "Cant create this in QML. Get it from Board.");
    qmlRegisterUncreatableType<TowerFactory>("Machines", 1, 0, "TowerFactory", "Cant create this in QML. Get it from Engine.");
    qmlRegisterType<TowerProxyModel>("Machines", 1, 0, "TowerProxyModel");
    qmlRegisterType<BoardProxyModel>("Machines", 1, 0, "BoardProxyModel");

    qmlRegisterSingletonType<Settings>("Machines", 1, 0, "SettingsBackend", createSettings);

}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    ImageProvider *imageProvider = new ImageProvider("boards");
    engine->addImageProvider("boards", imageProvider);
    imageProvider = new ImageProvider("packdata");
    engine->addImageProvider("packdata", imageProvider);
    imageProvider = new ImageProvider("enemies");
    engine->addImageProvider("enemies", imageProvider);
    imageProvider = new ImageProvider("towers");
    engine->addImageProvider("towers", imageProvider);

    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

