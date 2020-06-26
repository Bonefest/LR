#include "DirectionalAnimation.h"
#include <utility>

Frame DirectionalAnimation::getCurrentFrame() const {
    return Frame(m_currentFramePosition.x, m_currentFramePosition.y,
                 m_frameSize.x, m_frameSize.y);
}

bool DirectionalAnimation::nextFrame() {
    if(isFinished()) {
        return false;
    }

    if(m_direction == ROW && m_currentFramePosition.x == m_endFramePosition.x) {
        m_currentFramePosition.y += m_frameSize.y;
    }
    else if(m_direction == COLUMN && m_currentFramePosition.y == m_endFramePosition.y) {
        m_currentFramePosition.x += m_frameSize.x;
    }
    else {
        m_currentFramePosition += m_directionVector;
    }
    return true;
}

bool DirectionalAnimation::previousFrame() {
    if(m_currentFramePosition == m_startFramePosition) {
        return false;
    }

    m_currentFramePosition -= m_directionVector;
    return true;
}

void DirectionalAnimation::reset() {
    m_currentFramePosition = m_startFramePosition;
}

bool DirectionalAnimation::isFinished() const {
    return (m_currentFramePosition == m_endFramePosition);
}

bool DirectionalAnimation::initWithJson(nlohmann::json& parser) {

    m_direction = Direction(parser.value("direction", ROW));
    m_reversed  = parser.value("reversed", false);
    m_startFramePosition = sf::Vector2i(parser["start_frame"]["x"],
                                        parser["start_frame"]["y"]);

    m_endFramePosition = sf::Vector2i(parser["end_frame"]["x"],
                                      parser["end_frame"]["y"]);


    m_frameSize = sf::Vector2u(parser["size"]["w"], parser["size"]["h"]);

    calculateDirectionVector();

    return true;
}

void DirectionalAnimation::calculateDirectionVector() {
    if(m_direction == ROW) {
        m_directionVector = sf::Vector2i(m_frameSize.x, 0);
        if(m_reversed) {
            m_directionVector.x = -m_directionVector.x;
        }
    }
    else {
        m_directionVector = sf::Vector2i(0, m_frameSize.y);
        if(m_reversed) {
            m_directionVector.y = -m_directionVector.y;
        }

    }

    if(m_reversed) {
        std::swap(m_endFramePosition, m_startFramePosition);
    }
}

DirectionalAnimation* DirectionalAnimation::create() const {
    DirectionalAnimation* animation     = new DirectionalAnimation();
    animation->m_directionVector        = m_directionVector;
    animation->m_reversed               = m_reversed;
    animation->m_startFramePosition     = m_startFramePosition;
    animation->m_endFramePosition       = m_endFramePosition;
    animation->m_currentFramePosition   = m_startFramePosition;
    animation->m_frameSize              = m_frameSize;
    animation->m_frameTime              = m_frameTime;
    animation->m_loop                   = m_loop;
    animation->m_reversed               = m_reversed;
    animation->m_texture                = m_texture;

    return animation;
}



