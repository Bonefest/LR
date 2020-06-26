#include "StateManager.h"

StateManager::StateManager() { }
StateManager::~StateManager() {
    for(auto state : m_stack) {
        state.second->onDestroy();
        delete state.second;
    }
}

bool StateManager::hasState(StateType type) const {
    for(auto stackIt = m_stack.begin(); stackIt != m_stack.end(); stackIt++) {
        if(stackIt->first == type) {
            if(std::find(m_deleteRequestedStates.begin(),
                         m_deleteRequestedStates.end(),
                         type) == m_deleteRequestedStates.end()) {
                return true;
            }

            return false;
        }
    }

    return false;
}

void StateManager::setNextState(StateType stateType) {
    m_nextState = stateType;
}

bool StateManager::setState(StateType type) {
    if(std::find(m_deleteRequestedStates.begin(),
                 m_deleteRequestedStates.end(),
                 type) != m_deleteRequestedStates.end()) {
        return false;
    }

    m_context.eventManager->setCurrentState(type);

    for(auto stackIt = m_stack.begin(); stackIt != m_stack.end(); stackIt++) {
        // If we already have this kind of state - move it to the top
        if(stackIt->first == type) {
            auto pr = *stackIt;
            m_stack.erase(stackIt);
            if(!m_stack.empty()) {
                m_stack.back().second->onDeactivate();
            }

            m_stack.push_back(pr);
            m_stack.back().second->onActivate();

            return true;
        }
    }

    if(!m_stack.empty()) {
        m_stack.back().second->onDeactivate();
    }

    return createState(type);
}

bool StateManager::createState(StateType type) {
    auto factoryIt = m_stateFactory.find(type);
    if(factoryIt == m_stateFactory.end()) {
        return false;
    }

    BaseState* state = factoryIt->second();
    m_stack.emplace_back(type, state);
    state->onCreate();
    state->onActivate();

    return true;
}

void StateManager::removeState(StateType type) {
    m_deleteRequestedStates.push_back(type);
}

void StateManager::processRequests() {
    for(auto request : m_deleteRequestedStates) {
        deleteState(request);
    }

    m_deleteRequestedStates.clear();
}

void StateManager::deleteState(StateType type) {
    for(auto stackIt = m_stack.begin(); stackIt != m_stack.end(); stackIt++) {
        if(stackIt->first == type) {
            stackIt->second->onDestroy();
            m_stack.erase(stackIt);
            return;
        }
    }
}

void StateManager::update(const sf::Time& time) {
    if(m_stack.empty()) {
        return;
    }

    auto it = m_stack.end() - 1;
    for(;it != m_stack.begin(); it--) {
        if(!it->second->isTranscendent()) {
            break;
        }
    }

    for(;it != m_stack.end(); it++) {
        it->second->update(time);
    }

    if(m_nextState != StateType::SYSTEM) {
        setState(m_nextState);
        m_nextState = StateType::SYSTEM;
    }
}

void StateManager::draw() {
    if(m_stack.empty()) {
        return;
    }

    auto it = m_stack.end() - 1;
    for(;it != m_stack.begin(); it--) {
        if(!it->second->isTransparent()) {
            break;
        }
    }

    sf::RenderWindow* window = m_context.window->getWindow();

    for(;it != m_stack.end(); it++) {
        window->setView(it->second->getView());
        it->second->draw();
    }
}
