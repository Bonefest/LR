#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED

#include "Window.h"
#include "StateBase.h"
#include "StateType.h"

#include "ResourcesManager.h"
#include "AnimationManager.h"

#include "SystemsManager.h"

struct StateContext {
    Window*             window;
    EventManager*       eventManager;
    TextureManager*     textureManager;
    AnimationManager*   animationManager;
    SystemsManager*     systemsManager;
};

class BaseState;

using StateStack = vector<std::pair<StateType, BaseState*>>;
using StateFactory = map<StateType, std::function<BaseState*(void)>>;

class StateManager {
public:

    StateManager();
    ~StateManager();

    template <typename T>
    bool registerState(StateType type) {
        if(m_stateFactory.find(type) != m_stateFactory.end())
            return false;

        return m_stateFactory.emplace(type, [this]() {
            return new T(this);
        }).second;
    }

    bool hasState(StateType type) const;
    bool setState(StateType type);
    void setNextState(StateType);
    void removeState(StateType type);

    void processRequests();

    void update(const sf::Time& time);
    void draw();

    inline StateContext* getContext() { return &m_context; }

private:
    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    bool createState(StateType type);
    void deleteState(StateType type);

    StateStack          m_stack;
    vector<StateType>   m_deleteRequestedStates;
    StateFactory        m_stateFactory;
    StateType           m_nextState;

    StateContext        m_context;
};


#endif // STATEMANAGER_H_INCLUDED
