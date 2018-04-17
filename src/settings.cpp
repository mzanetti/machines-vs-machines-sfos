#include "settings.h"

#include <QSettings>
#include <QDebug>

Settings::Settings(QObject *parent):
    QObject(parent)
{
}

Settings::~Settings()
{
}

bool Settings::audioEnabled() const
{
    SETTINGS("global");
    return settings.value("audioEnabled", true).toBool();
}

void Settings::setAudioEnabled(bool audioEnabled)
{
    SETTINGS("global");
    if (this->audioEnabled() != audioEnabled) {
        settings.setValue("audioEnabled", audioEnabled);
        emit audioEnabledChanged();
    }
}

int Settings::volume() const
{
    SETTINGS("global");
    return settings.value("volume", 75).toInt();
}

void Settings::setVolume(int volume)
{
    SETTINGS("global");
    if (this->volume() != volume) {
        settings.setValue("volume", volume);
        emit volumeChanged();
    }
}

int Settings::effectsVolume() const
{
    SETTINGS("global");
    return settings.value("effectsVolume", 30).toInt();
}

void Settings::setEffectsVolume(int effectsVolume)
{
    SETTINGS("global");
    if (this->effectsVolume() != effectsVolume) {
        settings.setValue("effectsVolume", effectsVolume);
        emit effectsVolumeChanged();
    }
}

bool Settings::fieldOverlayEnabled() const
{
    SETTINGS("global");
    return settings.value("fieldOverlayEnabled", false).toBool();
}

void Settings::setFieldOverlayEnabled(bool fieldOverlayEnabled)
{
    SETTINGS("global");
    if (this->fieldOverlayEnabled() != fieldOverlayEnabled) {
        settings.setValue("fieldOverlayEnabled", fieldOverlayEnabled);
        emit fieldOverlayEnabledChanged();
    }
}

bool Settings::allUnlocked() const
{
    SETTINGS("global");
    return settings.value("allUnlocked", false).toBool();
}

void Settings::setAllUnlocked(bool allUnlocked)
{
    SETTINGS("global");
    if (this->allUnlocked() != allUnlocked) {
        settings.setValue("allUnlocked", allUnlocked);
        emit allUnlockedChanged();
    }
}

int Settings::lastUsedLevelPack() const
{
    SETTINGS("global");
    return settings.value("lastUsedLevelPack").toInt();
}

void Settings::setLastUsedLevelPack(int lastUsedLevelPack)
{
    SETTINGS("global");
    if (this->lastUsedLevelPack() != lastUsedLevelPack) {
        settings.setValue("lastUsedLevelPack", lastUsedLevelPack);
        emit lastUsedLevelPackChanged();
    }
}

int Settings::lastUsedLevelPage() const
{
    SETTINGS("global");
    return settings.value("lastUsedLevelPage").toInt();
}

void Settings::setLastUsedLevelPage(int lastUsedLevelPage)
{
    SETTINGS("global");
    if (this->lastUsedLevelPage() != lastUsedLevelPage) {
        settings.setValue("lastUsedLevelPage", lastUsedLevelPage);
        emit lastUsedLevelPageChanged();
    }
}

bool Settings::tutorialShown() const
{
    SETTINGS("global");
    return settings.value("tutorialShown", false).toBool();
}

void Settings::setTutorialShown(bool shown)
{
    SETTINGS("global");
    if (this->tutorialShown() != shown) {
        settings.setValue("tutorialShown", shown);
        emit tutorialShownChanged();
    }
}

void Settings::resetScores()
{
    resetHighscores();
    resetTowers();
}

void Settings::resetHighscores()
{
    SETTINGS("scores");
    settings.remove("");
}

void Settings::resetTowers()
{
    SETTINGS("towers");
    settings.remove("");
}
