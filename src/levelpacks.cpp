#include "levelpacks.h"
#include "levelpack.h"

#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QDebug>

LevelPacks::LevelPacks(Engine *engine, const QUrl &dataDir, QObject *parent) :
    QAbstractListModel(parent),
    m_engine(engine)
{
    QDir dir(dataDir.path());
    QStringList levelPacks = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
    qDebug() << "Loading level packs from:" << dir.path() << levelPacks;
    qDebug() << dir.entryList();
    foreach (const QString &levelPackDir, levelPacks) {
        QFileInfo fi(dir.absolutePath() + "/" + levelPackDir + "/levelpack.json");
        if (!fi.exists()) {
            qDebug() << "Level pack directory" << levelPackDir << "does not contain a levelpack.json file.";
            continue;
        }
        QFile jsonFile(fi.absoluteFilePath());
        if (!jsonFile.open(QFile::ReadOnly)) {
            qDebug() << "Cannot open level pack file for reading:" << fi.absoluteFilePath();
            continue;
        }
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll(), &error);
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "Cannot parse level pack file:" << error.errorString();
            continue;
        }
        QVariantMap jsonMap = jsonDoc.toVariant().toMap();
        QString name = jsonMap.value("name").toString();
        QString titleImage = jsonMap.value("titleImage").toString();
        QString copyright = jsonMap.value("copyright").toString();
        QString titleSound = jsonMap.value("titleSound").toString();
        LevelPack *pack = new LevelPack(m_engine, levelPackDir, name, titleImage, copyright, titleSound, this);
        qDebug() << "Loaded level pack:" << pack->id() << pack->name() << pack->titleImage();
        m_list.append(pack);
    }
}

QVariant LevelPacks::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleId:
        return m_list.at(index.row())->id();
    case RoleName:
        return m_list.at(index.row())->name();
    case RoleTitleImage:
        return m_list.at(index.row())->titleImage();
    case RoleCopyright:
        return m_list.at(index.row())->copyright();
    }
    return QVariant();
}

int LevelPacks::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

LevelPack *LevelPacks::levelPack(const QString &id) const
{
    foreach (LevelPack *pack, m_list) {
        if (pack->id() == id) {
            return pack;
        }
    }
    return nullptr;
}
