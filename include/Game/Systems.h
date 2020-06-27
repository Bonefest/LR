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

class ObstacleRenderingSystem: public System {
public:
    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        CamerasContext& context = registry.ctx<CamerasContext>();
        auto obstacleView = registry.view<Obstacle>();
        //Sort obstacle views from top-to-down from right-to-left
        //sort from left-top to right bottom
        obstacleView.each([&](entt::entity obstcle, Obstacle& obstacle) {

            if(obstacle.color == WHITE) {
                window.setView(*context.whiteView);
            }
            else {
                window.setView(*context.blackView);
            }

            sf::RenderWindow* rwindow = window.getWindow();
            rwindow->draw(obstacle.vertices, 18, sf::PrimitiveType::Triangles);
        });
    }
};

#endif // SYSTEMS_H_INCLUDED
