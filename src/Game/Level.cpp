
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


void Level::resetPlayers(entt::registry& registry, entt::dispatcher& dispatcher) {
    auto playersView = registry.view<Player>();
    playersView.each([&](entt::entity plr, Player& player) {
        if(player.color == WHITE) {
            player.gameMap = m_whiteMap;
        } else {
            player.gameMap = m_blackMap;
        }

        player.sprite->setPosition(player.gameMap->getStartPosition());
    });

}

entt::entity Level::createFlame(entt::registry& registry, const sf::Vector2f& position, PlayerColor color) {
    entt::entity flame = registry.create();

    std::shared_ptr<AnimatedSprite> sprite = std::make_shared<AnimatedSprite>();
    sprite->setAnimation("flame", m_manager->getAnimationManager()->getAnimation("flame"));
    sprite->activateAnimation("flame");
    sprite->setPosition(position);
    sprite->setOrigin(50.0f, 50.0f);

    PlayerStatePtr state = std::make_shared<FlameIdleState>(flame);

    registry.assign<Flame>(flame, sprite, color, state);

    return flame;
}



