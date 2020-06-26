#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <memory>
#include "entt.hpp"

#include <SFML/Graphics.hpp>

#include "Window.h"

class Window;

class System {
public:
    virtual void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) { }
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) { }
    virtual void onDestroy() { }
    virtual void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) { }


};

using SystemPtr = std::shared_ptr<System>;

#endif // SYSTEM_H_INCLUDED
