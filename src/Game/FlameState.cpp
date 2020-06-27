#include "Game/FlameState.h"

#include "Game/Components.h"

#include <cmath>
#include <iostream>
FlameIdleState::FlameIdleState(entt::entity flame): PlayerState(flame) { }

void FlameIdleState::onActivate(entt::registry& registry, entt::dispatcher& dispatcher) {
    Flame& flame = registry.get<Flame>(m_player);
    flame.sprite->activateAnimation("flame");
}

void FlameIdleState::update(entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const sf::Time& dt) {
    Flame& flame = registry.get<Flame>(m_player);
    m_elapsedTime += dt;

    float s = std::max(std::sin(m_elapsedTime.asSeconds()) * 0.20f + 0.20f, 0.1f);
    float c = std::max(std::cos(m_elapsedTime.asSeconds()) * 0.40f + 0.40f, 0.1f);

    flame.sprite->setScale(c, s);
}

FlameDeathState::FlameDeathState(entt::entity flame): PlayerState(flame) { }

void FlameDeathState::update(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const sf::Time& dt) {
    Flame& flame = registry.get<Flame>(m_player);

    m_elapsedTime += dt;
    float t = m_elapsedTime.asSeconds() / 0.5f;
    if(t >= 1.0f) {
        registry.destroy(m_player);
        return;
    }
    flame.sprite->setColor(sf::Color(255, 255, 255, 255 * (1.0f - t)));
}
