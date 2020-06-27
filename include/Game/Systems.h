#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "System.h"
#include "Components.h"
#include "Game/Events.h"

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
        playersView.each([&](entt::entity plr, Player& player){
            // CHECK IF THEY ARE SWAPPED
            if(player.color == WHITE) {
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

#endif // SYSTEMS_H_INCLUDED
