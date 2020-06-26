#ifndef MAINGAMESTATE_H_INCLUDED
#define MAINGAMESTATE_H_INCLUDED

#include "StateBase.h"
#include "Player.h"

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

    sf::View m_leftCamera;
    sf::View m_rightCamera;

};

#endif // MAINGAMESTATE_H_INCLUDED
