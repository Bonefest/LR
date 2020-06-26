#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "System.h"
#include "Player.h"
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
        playersView.each([&](entt::entity plr, Player& player){
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

#endif // SYSTEMS_H_INCLUDED
