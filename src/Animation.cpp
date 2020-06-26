#include "Animation.h"

void Animation::update(const sf::Time& dt) {
    if(m_frameTime <= sf::seconds(0.001)) {
        return;
    }

    m_elapsedTime += dt;
    while(m_elapsedTime >= m_frameTime) {
        if(!nextFrame() && isLoop()) {
            reset();
        }
        m_elapsedTime -= m_frameTime;

    }
}

const sf::Time& Animation::getFrameDuration() const {
    return m_frameTime;
}

void Animation::setFrameDuration(const sf::Time& time) {
    m_frameTime = time;
}

void Animation::setLoop(bool loop) {
    m_loop = loop;
}

bool Animation::isLoop() const {
    return m_loop;
}

