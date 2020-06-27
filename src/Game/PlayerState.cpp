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

static sf::Vector2i directionToVector(MovingDirection direction) {
    if(direction == LEFT) {
        return sf::Vector2i(-1, 0);
    }
    else if(direction == RIGHT) {
        return sf::Vector2i(1, 0);
    }
    else if(direction == TOP) {
        return sf::Vector2i(0, -1);
    }
    else if(direction == BOTTOM) {
        return sf::Vector2i(0, 1);
    }

    return sf::Vector2i(0, 0);
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

            if(key == sf::Keyboard::X) {
                tryMoveThrough(registry, dispatcher, player);
            }
        }
    }
    m_keys.clear();
}

void PlayerIdleState::onKeyEvent(KeyEvent event) {
    m_keys.push_back(event);
}


void PlayerIdleState::tryMoveThrough(entt::registry& registry, entt::dispatcher& dispatcher, Player& player) {
    if(player.color == WHITE) {
        return;
    }

    sf::Vector2i direction = directionToVector(player.lastMovingDirection);
    sf::Vector2i mapCoord = player.gameMap->convertToMapCoords(player.sprite->getPosition());
    mapCoord.x += direction.x;
    mapCoord.y -= direction.y;

    if(mapCoord.x < 0 || mapCoord.x >= Constants::mapSize.x - 1 ||
       mapCoord.y < 0 || mapCoord.y >= Constants::mapSize.y - 1) {
        return;
    }

    sf::Vector2f futurePos = player.sprite->getPosition();
    futurePos += float(direction.x) * Constants::i * Constants::platformSize.x * 1.5f;
    futurePos += float(direction.y) * Constants::j * Constants::platformSize.y * 1.5f;

    if(player.gameMap->isSet(futurePos)) {
        setState(registry, dispatcher, std::make_shared<PlayerMovingThroughState>(m_player));
    }
}





PlayerMovingState::PlayerMovingState(entt::entity player,
                                     sf::Keyboard::Key key,
                                     MovingDirection direction): PlayerState(player),
                                                                 m_key(key),
                                                                 m_direction(direction) { }

void PlayerMovingState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {

    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("move");
    player.lastMovingDirection = m_direction;

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


PlayerMovingThroughState::PlayerMovingThroughState(entt::entity player): PlayerState(player) { }

void PlayerMovingThroughState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {
    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("move_through");
    m_startPoint = player.sprite->getPosition();

    sf::Vector2i direction = directionToVector(player.lastMovingDirection);

    m_endPoint = Constants::i * float(direction.x) * Constants::platformSize.x * 1.5f;
    m_endPoint += Constants::j * float(direction.y) * Constants::platformSize.y * 1.5f;
    m_endPoint += m_startPoint;

}

void PlayerMovingThroughState::update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
    m_elapsedTime += dt;

    Player& player = registry.get<Player>(m_player);

    float t = m_elapsedTime.asSeconds() / player.sprite->getCurrentAnimation()->getFrameDuration().asSeconds();
    if(t >= 1.0f) {
        setState(registry, dispatcher, std::make_shared<PlayerIdleState>(m_player));
    }

    player.sprite->setPosition(m_startPoint * (1.0f - t) + m_endPoint * t);
}
