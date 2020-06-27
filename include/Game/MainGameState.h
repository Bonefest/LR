#ifndef MAINGAMESTATE_H_INCLUDED
#define MAINGAMESTATE_H_INCLUDED

#include "StateBase.h"
#include "Components.h"

#include "Game/GameMap.h"
#include "Game/Level.h"

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
    entt::entity createFlame(const sf::Vector2f& position, PlayerColor);

    sf::View        m_blackCamera;
    sf::View        m_whiteCamera;
    sf::View        m_uiCamera;

    entt::entity    m_black;
    entt::entity    m_white;

    GameMap*        m_blackMap;
    GameMap*        m_whiteMap;

    LevelManager    m_levelManager;

};

#endif // MAINGAMESTATE_H_INCLUDED
