#include "StateBase.h"

BaseState::BaseState(StateManager* manager,
                     bool transparent,
                     bool transcedent): m_smanager(manager),
                                        m_transparent(transparent),
                                        m_transcendent(transcedent) { }

void BaseState::setTransparent(bool transparent) {
    m_transparent = transparent;
}

void BaseState::setTranscendent(bool transcedent) {
    m_transcendent = transcedent;
}

const sf::View& BaseState::getView() const {
    return m_view;
}
