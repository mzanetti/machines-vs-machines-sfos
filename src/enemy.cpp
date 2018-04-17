#include "enemy.h"
#include "board.h"
#include "field.h"
#include "engine.h"

Enemy::Enemy(Engine *engine) :
    QObject(engine->board()),
    m_engine(engine),
    m_board(engine->board()),
    m_running(false),
    m_progress(0),
    m_travelledDistance(0),
    m_speed(0),
    m_energy(1)
{
    m_upcomingFields = m_board->path();
    if (m_upcomingFields.count() > 0) {
        m_currentField = m_upcomingFields.takeFirst();
    } else {
        m_currentField = TravelStatusEntry;
    }
    if (m_upcomingFields.count() > 0) {
        m_nextField = m_upcomingFields.takeFirst();
    } else {
        m_nextField = TravelStatusExit;
    }

    if (m_nextField == m_currentField + 1) {
        m_walkingDirection = WalkingDirectionRight;
    } else if (m_nextField == m_currentField + m_board->columns()) {
        m_walkingDirection = WalkingDirectionDown;
    } else if (m_nextField == m_currentField - m_board->columns()) {
        m_walkingDirection = WalkingDirectionUp;
    } else {
        m_walkingDirection = WalkingDirectionLeft;
    }

    connect(m_engine, &Engine::tick, this, &Enemy::tick);
}

QString Enemy::image() const
{
    return m_image;
}

int Enemy::imageBaseSize() const
{
    return m_imageBaseSize;
}

void Enemy::setImage(const QString &image, int baseSize, int spriteCount, int animationDuration)
{
    if (m_image != image) {
        m_image = image;
        m_imageBaseSize = baseSize;
        m_spriteCount = spriteCount;
        m_animationDuration = animationDuration;
        emit imageChanged();
    }
}

int Enemy::spriteCount() const
{
    return m_spriteCount;
}

int Enemy::animationDuration() const
{
    return m_animationDuration;
}


int Enemy::x() const
{
    switch (m_currentField) {
    case TravelStatusEntry:
        return m_nextField % m_board->columns();
    }

    return m_currentField % m_board->columns();
}

int Enemy::y() const
{
    switch (m_currentField) {
    case TravelStatusEntry:
        return m_nextField / m_board->columns();
    }
    return m_currentField / m_board->columns();
}

int Enemy::currentField() const
{
    return m_currentField;
}

int Enemy::nextX() const
{
    switch (m_nextField) {
    case TravelStatusEntry:
        return -1;
    case TravelStatusExit:
        return m_currentField % m_board->columns();
    }

    return m_nextField % m_board->columns();
}

int Enemy::nextY() const
{
    switch (m_nextField) {
    case TravelStatusEntry:
        return m_board->path().first() / m_board->columns();
    case TravelStatusExit:
        return m_currentField / m_board->columns();
    }

    return m_nextField / m_board->columns();
}

int Enemy::nextField() const
{
    return m_nextField;
}

bool Enemy::running() const
{
    return m_running;
}

void Enemy::setRunning(bool running)
{
    if (m_running != running) {
        m_running = running;
        emit runningChanged();
    }
}

qreal Enemy::progress() const
{
    return m_progress;
}

void Enemy::advance()
{
    if (m_currentField == TravelStatusExit) {
        qDebug() << "already exited. not advancing";
        return;
    }

    m_currentField = m_nextField;
    m_travelledFields.append(m_currentField);
    if (m_upcomingFields.count() > 0) {
        m_nextField = m_upcomingFields.takeFirst();
    } else {
        m_nextField = TravelStatusExit;
    }

    WalkingDirection oldDirection = m_walkingDirection;
    if (m_nextField == TravelStatusExit) {
        m_running = false;
        emit exited();
        emit runningChanged();
    } else if (m_nextField == m_currentField -1) {
        m_walkingDirection = WalkingDirectionLeft;
    } else if (m_nextField < m_currentField) {
        m_walkingDirection = WalkingDirectionUp;
    } else if (m_nextField % m_board->columns() == m_currentField % m_board->columns()) {
        m_walkingDirection = WalkingDirectionDown;
    } else {
        m_walkingDirection = WalkingDirectionRight;
    }
    if (m_walkingDirection != oldDirection) {
        emit walkingDirectionChanged();
    }
    emit currentFieldChanged();
}

void Enemy::hit(int damage, int slowDown, int delay)
{
//    qDebug() << "taking hit" << damage << slowDown;
    Hit hit;
    hit.damage = damage;
    hit.slowDown = slowDown;
    // tps : 1000 = ticks : delay
    int ticks = delay * m_engine->ticksPerSecond() / 1000;
    m_hitList.append(qMakePair<int, Hit>(ticks, hit));
}

void Enemy::tick()
{
    if (m_energy <= 0) {
        return; // Not moving... dead...
    }

    for (int i = 0; i < m_hitList.count(); i++) {
        m_hitList[i].first -= m_engine->gameSpeed();
        if (m_hitList[i].first <= 0) {
            executeHit(m_hitList[i].second);
            m_hitList.removeAt(i--);
        }
    }

    for (int i = 0; i < m_slowdowns.count(); i++) {
        m_slowdowns[i].first -= m_engine->gameSpeed();
        if (m_slowdowns[i].first <= 0) {
            m_speed += m_slowdowns[i].second;
            emit speedChanged();
            m_slowdowns.removeAt(i--);
        }
    }

    if (!m_running) {
        return;
    }
    qreal diff = (m_engine->stepSize() + 0.001 * m_speed) * m_engine->gameSpeed();
    m_progress += diff;
    m_travelledDistance += diff;
    if (m_progress >= 1) {
        advance();
        m_progress = 0;
    }
    emit progressChanged();
}

void Enemy::executeHit(const Hit &hit)
{
    if (hit.damage > 0) {
        m_energy = qMax(0, m_energy - hit.damage);
        emit energyChanged(m_energy);
        if (m_energy <= 0) {
            m_hitList.clear();
            emit died();
            m_running = false;
            emit runningChanged();
        }
    }

    if (hit.slowDown > 0) {
        m_speed -= hit.slowDown;
        emit speedChanged();
        qDebug() << "speed changed" << m_speed << m_engine->ticksPerSecond() << hit.slowDown;
        m_slowdowns.append(qMakePair<int, int>(4 * m_engine->ticksPerSecond(), hit.slowDown));
    }
}

int Enemy::speed() const
{
    return m_speed;
}

void Enemy::setSpeed(int speed)
{
    m_speed = speed;
    emit speedChanged();
}

void Enemy::setEnergy(int energy)
{
    m_energy = energy;
    m_maxEnergy = energy;
    emit maxEnergyChanged();
    emit energyChanged(m_energy);
}

int Enemy::reward() const
{
    return m_reward;
}

void Enemy::setReward(int reward)
{
    m_reward = reward;
}

int Enemy::energy() const
{
    return m_energy;
}

int Enemy::maxEnergy() const
{
    return m_maxEnergy;
}

qreal Enemy::travelledDistance() const
{
    return m_travelledDistance;
}

Enemy::WalkingDirection Enemy::walkingDirection() const
{
    return m_walkingDirection;
}
