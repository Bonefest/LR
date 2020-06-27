#ifndef PLAYERSTATE_H_INCLUDED
#define PLAYERSTATE_H_INCLUDED

#include <memory>

#include "entt.hpp"
#include "Window.h"

#include <SFML/Graphics.hpp>

#include "Game/Events.h"

#include "Constants.h"

class Player;
class PlayerState;

using PlayerStatePtr = std::shared_ptr<PlayerState>;

class PlayerState {
public:
	PlayerState(entt::entity player);

    virtual void onActivate(entt::registry& registry, entt::dispatcher& dispatcher) { }
    virtual void onDeactivate(entt::registry& registry, entt::dispatcher& dispatcher) { }

	virtual void update(entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const sf::Time& dt) { }
	virtual void onKeyEvent(KeyEvent event) { }

	void setState(entt::registry& registry,
                  entt::dispatcher& dispatcher,
                  PlayerStatePtr state);

protected:
	entt::entity    m_player;
};

class PlayerIdleState: public PlayerState {
public:
    PlayerIdleState(entt::entity player);

    virtual void onActivate(entt::registry& registry, entt::dispatcher& dispatcher);

	virtual void update(entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const sf::Time& dt);
	virtual void onKeyEvent(KeyEvent event);
private:
    void tryMoveThrough(entt::registry& registry, entt::dispatcher& dispatcher, Player& player);

    std::vector<KeyEvent> m_keys;
};

class PlayerMovingState: public PlayerState {
public:
    PlayerMovingState(entt::entity player, sf::Keyboard::Key key, MovingDirection direction);

    virtual void onActivate(entt::registry& registry, entt::dispatcher& dispatcher);

	virtual void update(entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const sf::Time& dt);

	virtual void onKeyEvent(KeyEvent event);

private:
    sf::Keyboard::Key   m_key;
    MovingDirection     m_direction;
    sf::Vector2f        m_velocity;

    std::vector<KeyEvent> m_keys;
};

class PlayerMovingThroughState: public PlayerState {
public:
    PlayerMovingThroughState(entt::entity player);

    virtual void onActivate(entt::registry& registry, entt::dispatcher& dispatcher);

	virtual void update(entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const sf::Time& dt);

private:
    sf::Vector2f    m_endPoint;
    sf::Vector2f    m_startPoint;

    sf::Time        m_elapsedTime;
};


#endif // PLAYERSTATE_H_INCLUDED
