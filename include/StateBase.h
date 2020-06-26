#ifndef STATEBASE_H_INCLUDED
#define STATEBASE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "StateManager.h"

class StateManager;

class BaseState {
    friend class StateManager;
public:

    BaseState(StateManager* manager,
              bool transparent = false,
              bool transcendent = false);

    virtual ~BaseState() { }

    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;

    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;

    virtual void draw() = 0;
    virtual void update(const sf::Time& dt) = 0;

    inline bool isTransparent() const { return m_transparent; }
    void setTransparent(bool transparent);

    inline bool isTranscendent() const { return m_transcendent; }
    void setTranscendent(bool transcendent);

    const sf::View& getView() const;

protected:
    StateManager*   m_smanager;
    bool            m_transparent;
    bool            m_transcendent;

    sf::View        m_view;

};

#endif // STATEBASE_H_INCLUDED
