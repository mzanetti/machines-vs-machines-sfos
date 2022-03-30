#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#define SETTINGS(group) QSettings settings("com.github.mzanetti/harbour-machines-vs-machines-sfos/harbour-machines-vs-machines-sfos"); settings.beginGroup(group)

class QSettings;

class Settings: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool audioEnabled READ audioEnabled WRITE setAudioEnabled NOTIFY audioEnabledChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(int effectsVolume READ effectsVolume WRITE setEffectsVolume NOTIFY effectsVolumeChanged)
    Q_PROPERTY(bool fieldOverlayEnabled READ fieldOverlayEnabled WRITE setFieldOverlayEnabled NOTIFY fieldOverlayEnabledChanged)
    Q_PROPERTY(bool allUnlocked READ allUnlocked WRITE setAllUnlocked NOTIFY allUnlockedChanged)
    Q_PROPERTY(bool tutorialShown READ tutorialShown WRITE setTutorialShown NOTIFY tutorialShownChanged)

    Q_PROPERTY(int lastUsedLevelPack READ lastUsedLevelPack WRITE setLastUsedLevelPack NOTIFY lastUsedLevelPackChanged)
    Q_PROPERTY(int lastUsedLevelPage READ lastUsedLevelPage WRITE setLastUsedLevelPage NOTIFY lastUsedLevelPageChanged)

public:
    Settings(QObject *parent = nullptr);
    ~Settings();

    bool audioEnabled() const;
    void setAudioEnabled(bool audioEnabled);

    int volume() const;
    void setVolume(int volume);

    int effectsVolume() const;
    void setEffectsVolume(int effectsVolume);

    bool fieldOverlayEnabled() const;
    void setFieldOverlayEnabled(bool fieldOverlayEnabled);

    bool allUnlocked() const;
    void setAllUnlocked(bool allUnlocked);

    int lastUsedLevelPack() const;
    void setLastUsedLevelPack(int lastUsedLevelPack);

    int lastUsedLevelPage() const;
    void setLastUsedLevelPage(int lastUsedLevelPage);

    bool tutorialShown() const;
    void setTutorialShown(bool shown);

    void resetScores();

private:
    void resetHighscores();
    void resetTowers();

signals:
    void audioEnabledChanged();
    void volumeChanged();
    void effectsVolumeChanged();
    void fieldOverlayEnabledChanged();
    void allUnlockedChanged();
    void tutorialShownChanged();

    void lastUsedLevelPackChanged();
    void lastUsedLevelPageChanged();
};

#endif
