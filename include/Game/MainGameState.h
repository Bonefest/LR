#ifndef MAINGAMESTATE_H_INCLUDED
#define MAINGAMESTATE_H_INCLUDED

#include "StateBase.h"
#include "Components.h"

class MainGameState: public BaseState {
public:

    MainGameState(StateManager* manager);

    virtual ~MainGameState() { }

    virtual void onCreate();
    virtual void onDestroy();

    virtual void onActivate();
    virtual void onDeactivate();

    virtual void draw();
    virtual void update(const sf::Time& dt);

private:
    entt::entity createPlayer(PlayerColor);

    sf::View        m_blackCamera;
    sf::View        m_whiteCamera;

    entt::entity    m_black;
    entt::entity    m_white;

};

#endif // MAINGAMESTATE_H_INCLUDED
