#include "Game/PlayerState.h"
#include "Game/Components.h"

#include "Game/Constants.h"

static MovingDirection getDirection(sf::Keyboard::Key key, PlayerColor color) {
    if(key == sf::Keyboard::A) {
        return (color == PlayerColor::WHITE) ? LEFT : RIGHT;
    }
    else if(key == sf::Keyboard::D) {
        return (color == PlayerColor::WHITE) ? RIGHT : LEFT;
    }
    else if(key == sf::Keyboard::W) {
        return (color == PlayerColor::WHITE) ? BOTTOM : TOP;
    }
    else if(key == sf::Keyboard::S) {
        return (color == PlayerColor::WHITE) ? TOP : BOTTOM;
    }

    return NONE;
}

PlayerState::PlayerState(entt::entity player): m_player(player) { }

void PlayerState::setState(entt::registry& registry,
                           entt::dispatcher& dispatcher,
                           PlayerStatePtr state) {
    Player& player = registry.get<Player>(m_player);
    player.currentState->onDeactivate(registry, dispatcher);
    player.currentState = state;
    player.currentState->onActivate(registry, dispatcher);
}





PlayerIdleState::PlayerIdleState(entt::entity player): PlayerState(player) { }

void PlayerIdleState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {
    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("idle");
    player.sprite->resetAnimation();
}

void PlayerIdleState::update(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const sf::Time& dt) {

    Player& player = registry.get<Player>(m_player);

    for(auto _key : m_keys) {
        if(_key.pressed) {
            sf::Keyboard::Key key = _key.key;
            if(key == sf::Keyboard::A || key == sf::Keyboard::D ||
               key == sf::Keyboard::W || key == sf::Keyboard::S) {
                setState(registry,
                         dispatcher,
                         std::make_shared<PlayerMovingState>(m_player,
                                                             key,
                                                             getDirection(key, player.color)));
            }
        }
    }
    m_keys.clear();
}

void PlayerIdleState::onKeyEvent(KeyEvent event) {
    m_keys.push_back(event);
}






PlayerMovingState::PlayerMovingState(entt::entity player,
                                     sf::Keyboard::Key key,
                                     MovingDirection direction): PlayerState(player),
                                                                 m_key(key),
                                                                 m_direction(direction) { }

void PlayerMovingState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {

    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("move");

    const float SPEED = 100.0f;

    if(m_direction == LEFT) {
        player.sprite->setFlipped(true);
        m_velocity = sf::Vector2f(-SPEED, 0.0f);
    }
    else if(m_direction == RIGHT) {
        player.sprite->setFlipped(false);
        m_velocity = sf::Vector2f( SPEED, 0.0f);
    }
    else if(m_direction == TOP) {
        player.sprite->setFlipped(true);
        m_velocity = sf::Vector2f( 0.0, -SPEED);
    }
    else if(m_direction == BOTTOM) {
        player.sprite->setFlipped(false);
        m_velocity = sf::Vector2f( 0.0, SPEED);
    }

}
#include <iostream>
void PlayerMovingState::update(entt::registry& registry,
                               entt::dispatcher& dispatcher,
                               const sf::Time& dt) {
    Player& player = registry.get<Player>(m_player);


    sf::Vector2f nextPos = player.sprite->getPosition() + m_velocity.x * dt.asSeconds() * Constants::i + m_velocity.y * dt.asSeconds() * Constants::j;

    if(player.gameMap->isSet(nextPos)) {
        player.sprite->setPosition(nextPos);
    }

    for(auto key: m_keys) {
        if(!key.pressed && key.key == m_key) {
            setState(registry, dispatcher, std::make_shared<PlayerIdleState>(m_player));
        }
    }

    m_keys.clear();
}

void PlayerMovingState::onKeyEvent(KeyEvent event) {
    m_keys.push_back(event);
}

