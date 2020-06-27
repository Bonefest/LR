#ifndef FLAMESTATE_H_INCLUDED
#define FLAMESTATE_H_INCLUDED

#include "Game/PlayerState.h"

class FlameIdleState: public PlayerState {
public:
    FlameIdleState(entt::entity flame);

    void onActivate(entt::registry& registry, entt::dispatcher& dispatcher);

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt);

private:
    sf::Time m_elapsedTime;
};

class FlameDeathState: public PlayerState {
public:
    FlameDeathState(entt::entity flame);

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt);
private:
    sf::Time    m_elapsedTime;
};

#endif // FLAMESTATE_H_INCLUDED
