
#include "Game/Level.h"

LevelManager::LevelManager(): m_currentLevel(nullptr) { }

void LevelManager::update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
    if(m_currentLevel != nullptr) {
        m_currentLevel->update(registry, dispatcher, dt);
    }
}

void LevelManager::setLevel(entt::registry& registry, entt::dispatcher& dispatcher,std::shared_ptr<Level> level) {
    if(m_currentLevel != nullptr) {
        m_currentLevel->onDestroy(registry, dispatcher);
    }
    clearPreviousLevel(registry, dispatcher);
    m_currentLevel = level;
    m_currentLevel->onLoad(registry, dispatcher);
}

void LevelManager::clearPreviousLevel(entt::registry& registry, entt::dispatcher& dispatcher) {
    GameData& data = registry.ctx<GameData>();
    data.health = 3;

    auto flamesView = registry.view<Flame>();
    for(entt::entity flame: flamesView) {
        registry.destroy(flame);
    }

    GameMaps& maps = registry.ctx<GameMaps>();
    delete maps.whiteMap;
    delete maps.blackMap;
}


