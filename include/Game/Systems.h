#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "System.h"
#include "Components.h"
#include "Game/Events.h"

#include "Game/FlameState.h"

class PlayerAnimationsControllSystem: public System {
public:
    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        auto playersView = registry.view<Player>();
        playersView.each([&](entt::entity plr, Player& player) {
            player.currentState->update(registry, dispatcher, dt);
            player.sprite->update(dt);
        });
    }

    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        auto playersView = registry.view<Player>();
        CamerasContext& cameras = registry.ctx<CamerasContext>();
        GameMaps& gameMaps = registry.ctx<GameMaps>();
        playersView.each([&](entt::entity plr, Player& player){
            if(player.gameMap == gameMaps.whiteMap) {
                window.setView(*cameras.whiteView);
            }
            else {
                window.setView(*cameras.blackView);
            }

            window.draw(*player.sprite);

        });
    }
};

class KeyEventsNotifier: public System {
public:
    void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.sink<KeyEvent>().connect<&KeyEventsNotifier::onKeyEvent>(*this);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        auto playersView = registry.view<Player>();
        playersView.each([&](entt::entity plr, Player& player){
            for(auto key: m_keys) {
                player.currentState->onKeyEvent(key);
            }
        });

        m_keys.clear();
    }

    void onKeyEvent(const KeyEvent& event) {
        m_keys.push_back(event);
    }

private:
    std::vector<KeyEvent> m_keys;
};

class GameMapsRenderingSystem: public System {
public:
    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        CamerasContext& context = registry.ctx<CamerasContext>();
        GameMaps& gameMaps = registry.ctx<GameMaps>();

        window.setView(*context.whiteView);
        gameMaps.whiteMap->draw(window);

        window.setView(*context.blackView);
        gameMaps.blackMap->draw(window);
    }
};

class FlameRenderingSystem: public System {
public:

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        auto flamesView = registry.view<Flame>();
        flamesView.each([&](entt::entity flm, Flame& flame){
            flame.sprite->update(dt);
            flame.state->update(registry, dispatcher, dt);
        });
    }

    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        auto flamesView = registry.view<Flame>();
        CamerasContext cameras = registry.ctx<CamerasContext>();
        flamesView.each([&](entt::entity flm, Flame& flame){
            if(flame.color == BLACK) {
                window.setView(*cameras.blackView);
            } else {
                window.setView(*cameras.whiteView);
            }

            window.draw(*flame.sprite);
        });
    }
};
#include <iostream>
class AttackSystem: public System {
public:

    void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.sink<PlayerAttackEvent>().connect<&AttackSystem::onPlayerAttackEvent>(*this);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        GameMaps& gameMaps = registry.ctx<GameMaps>();
        for(auto event: m_events) {
            Player& player = registry.get<Player>(event.player);
            sf::Vector2f playerDirection = player.sprite->isFlipped() ? sf::Vector2f(-1.0f, 0.0f) : sf::Vector2f(1.0f, 0.0f);
            auto flamesView = registry.view<Flame>();

            PlayerColor actualColor = WHITE;
            if(player.gameMap == gameMaps.blackMap) {
                actualColor = BLACK;
            }


            flamesView.each([&](entt::entity flm, Flame& flame){
                if(flame.color == actualColor) {
                    sf::Vector2f relativePos = flame.sprite->getPosition() - player.sprite->getPosition();
                    float length = std::sqrt(relativePos.x * relativePos.x + relativePos.y * relativePos.y);
                    if(length > 0.001f) {

                        // Normalize
                        relativePos.x /= length;
                        relativePos.y /= length;
                        // Dot product
                        if(length < 45.0f && playerDirection.x * relativePos.x + playerDirection.y * relativePos.y > 0.0f) {
                            flame.state = std::make_shared<FlameDeathState>(flm);
                            return;
                        }
                    }
                }
            });


        }

        m_events.clear();
    }

    void onPlayerAttackEvent(const PlayerAttackEvent& event) {
        m_events.push_back(event);
    }

private:
    std::vector<PlayerAttackEvent> m_events;

};

#endif // SYSTEMS_H_INCLUDED
