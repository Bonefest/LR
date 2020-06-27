#include "Game/PlayerState.h"
#include "Game/Components.h"

#include "Game/Constants.h"

#include "Game/Events.h"

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
    player.lastPosition = player.sprite->getPosition();
    player.idle += dt;

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
            else if(key == sf::Keyboard::Z) {
                tryAttack(registry, dispatcher, player);
            }
            else if(key == sf::Keyboard::Space && m_keys.size() == 1) {
                tryTransform(registry, dispatcher, player);
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
        player.idle = sf::seconds(0.0f);
        setState(registry, dispatcher, std::make_shared<PlayerMovingThroughState>(m_player));
    }
}

void PlayerIdleState::tryAttack(entt::registry& registry,
                                entt::dispatcher& dispatcher,
                                Player& player) {
    if(player.color == WHITE) {
        player.idle = sf::seconds(0.0f);
        setState(registry, dispatcher, std::make_shared<PlayerAttackState>(m_player));
    }

}

void PlayerIdleState::tryTransform(entt::registry& registry,
                                   entt::dispatcher& dispatcher,
                                   Player& player) {
    auto playersView = registry.view<Player>();
    bool idle = player.idle.asSeconds() > 0.5f;
    playersView.each([&](entt::entity plr, Player& otherPlayer){
        idle = idle && (otherPlayer.idle.asSeconds() > 0.5f);
    });

    if(idle) {
        //trigger on transform event for particles
        setState(registry, dispatcher, std::make_shared<PlayerTransformState>(m_player));
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
    player.sprite->setPosition(nextPos);
    if(!player.gameMap->isSet(nextPos)) {
        if(m_direction == LEFT || m_direction == BOTTOM) {
            player.sprite->setColor(sf::Color(255, 255, 255, 0));
        }
        setState(registry, dispatcher, std::make_shared<PlayerFallingState>(m_player));
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


PlayerAttackState::PlayerAttackState(entt::entity player): PlayerState(player) { }

void PlayerAttackState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {
    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("attack");
    player.sprite->resetAnimation();
}

void PlayerAttackState::update(entt::registry& registry,
                               entt::dispatcher& dispatcher,
                               const sf::Time& dt) {
    Player& player = registry.get<Player>(m_player);
    if(player.sprite->getCurrentAnimation()->isFinished()) {
        dispatcher.trigger<PlayerAttackEvent>(m_player);
        setState(registry, dispatcher, std::make_shared<PlayerIdleState>(m_player));
    }
}




PlayerTransformState::PlayerTransformState(entt::entity player): PlayerState(player) { }

void PlayerTransformState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {
    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("transform");
    player.sprite->resetAnimation();
    m_animationFinished = false;
}

void PlayerTransformState::update(entt::registry& registry,
                                  entt::dispatcher& dispatcher,
                                  const sf::Time& dt) {
    Player& player = registry.get<Player>(m_player);
    if(m_animationFinished) {
        m_elapsedTime += dt;
        float t = m_elapsedTime.asSeconds() / 0.495f;
        sf::Color playerColor = player.sprite->getColor();
        if(t >= 1.0f) {
            playerColor.a = 255;
            player.sprite->setColor(playerColor);
            setState(registry, dispatcher, std::make_shared<PlayerIdleState>(m_player));
            return;
        }

        playerColor.a = std::min<sf::Uint8>(t * 255, 255);
        player.sprite->setColor(playerColor);

    } else {
        if(player.sprite->getCurrentAnimation()->isFinished()) {
            player.sprite->activateAnimation("idle");
            sf::Color playerColor = player.sprite->getColor();
            playerColor.a = 0;
            playerColor.r = 255;
            playerColor.g = 255;
            playerColor.b = 255;
            player.sprite->setColor(playerColor);
            m_animationFinished = true;

            GameMaps& maps = registry.ctx<GameMaps>();
            auto playersView = registry.view<Player>();
            playersView.each([&](entt::entity plr, Player& otherPlayer){
                if(plr != m_player) {
                    player.sprite->setPosition(otherPlayer.lastPosition);
                    if(player.gameMap == maps.blackMap) {
                        player.gameMap = maps.whiteMap;
                    } else {
                        player.gameMap = maps.blackMap;
                    }
                }
            });
        } else {
            player.sprite->setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }
    }
}

PlayerFallingState::PlayerFallingState(entt::entity player): PlayerState(player) { }

void PlayerFallingState::onActivate(entt::registry& registry,
                                    entt::dispatcher& dispatcher) {
    Player& player = registry.get<Player>(m_player);
    player.sprite->activateAnimation("idle");
    player.falling = true;
    player.idle = sf::seconds(0.0f);
}

void PlayerFallingState::onDeactivate(entt::registry& registry,
                                      entt::dispatcher& dispatcher) {
    Player& player = registry.get<Player>(m_player);
    player.sprite->setColor(sf::Color(255, 255, 255, 255));
    player.falling = false;
}

void PlayerFallingState::update(entt::registry& registry,
                                entt::dispatcher& dispatcher,
                                const sf::Time& dt) {
    Player& player = registry.get<Player>(m_player);
    GameData& data = registry.ctx<GameData>();


    m_elapsedTime += dt;
    if(m_elapsedTime.asSeconds() > 1.0f) {

        dispatcher.trigger<PlayerFallEvent>(m_player);

        if(data.health <= 1) {
            auto playersView = registry.view<Player>();
            playersView.each([&](entt::entity plr, Player& l_player){
                l_player.sprite->setPosition(player.gameMap->getStartPosition());
                l_player.sprite->setColor(sf::Color(0, 0, 0, 0));
                l_player.currentState->setState(registry, dispatcher, std::make_shared<PlayerDeathState>(m_player));
            });

            return;
        }
        player.sprite->setPosition(player.gameMap->getStartPosition());
        setState(registry, dispatcher, std::make_shared<PlayerIdleState>(m_player));
    }

    player.sprite->setPosition(player.sprite->getPosition() + sf::Vector2f(0.0f, 250.0f) * dt.asSeconds());
}

PlayerDeathState::PlayerDeathState(entt::entity player): PlayerState(player) { }

void PlayerDeathState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {
    dispatcher.trigger<TipEvent>("You lose! Press [R] to restart.", sf::seconds(5.0f), 10);
}
