#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED


#include "Game/FlameState.h"
#include "Game/Components.h"
#include "Game/GameMap.h"

#include "AnimationManager.h"

#include <memory>

class Level;

class LevelManager {
public:

    LevelManager();

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt);
    void setLevel(entt::registry& registry, entt::dispatcher& dispatcher,std::shared_ptr<Level> level);
    void clearPreviousLevel(entt::registry& registry, entt::dispatcher& dispatcher);

    void setAnimationManager(AnimationManager* manager) {
        m_animManager = manager;
    }
    AnimationManager* getAnimationManager() {
        return m_animManager;
    }
private:
    std::shared_ptr<Level>  m_currentLevel;
    AnimationManager*       m_animManager;
};

class Level {
public:

    Level(LevelManager* manager): m_manager(manager) { }

    virtual void onLoad(entt::registry& registry, entt::dispatcher& dispatcher) { }

    virtual void onDestroy(entt::registry& registry, entt::dispatcher& dispatcher) { }

    virtual void update(entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const sf::Time& dt) { }

    void resetPlayers(entt::registry& registry, entt::dispatcher& dispatcher) {
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


    entt::entity createFlame(entt::registry& registry, const sf::Vector2f& position, PlayerColor color) {
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

protected:
    GameMap*        m_whiteMap;
    GameMap*        m_blackMap;

    LevelManager*   m_manager;
};

class TutorialLevel: public Level {
public:
    TutorialLevel(LevelManager* manager): Level(manager) { }

    virtual void onLoad(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.trigger<TipEvent>("          This world is broken.\n"
                                               "It seems like you know now how to connect\n"
                                               "          to parallel university.", sf::seconds(4.0f), 5);
        dispatcher.trigger<TipEvent>("Use [WASD] to move around", sf::seconds(4.0f),5);
        dispatcher.trigger<TipEvent>("   Wait! All your actions are\n"
                                               "connected to parallel university.\n", sf::seconds(3.0f), 5);
        dispatcher.trigger<TipEvent>("Be sure you know what you are doing...\n", sf::seconds(3.0f), 5);
        dispatcher.trigger<TipEvent>("Use [X] for sprint (Black) and [Z] for attacking (White) \n", sf::seconds(4.0f), 5);
        dispatcher.trigger<TipEvent>("Use [Space] for swapping between universities\n", sf::seconds(4.0f), 5);
        dispatcher.trigger<TipEvent>("To save world you need to find and close all black flames!\n", sf::seconds(4.0f), 5);
        dispatcher.trigger<TipEvent>("Use all your power. Good luck. Game starts in a second ... \n", sf::seconds(4.0f), 5);

        m_whiteMap = new GameMap(WHITE);
        m_blackMap = new GameMap(BLACK);

        for(int i = 0;i < 10; ++i) {
            for(int j = 0;j < 10; ++j) {
                m_whiteMap->setPlatform(i, j);
            }
        }

        m_whiteMap->setStartPosition(m_whiteMap->convertToGlobalCoords(sf::Vector2i(5, 5)));

        for(int i = 0;i < 10; ++i) {
            for(int j = 0;j < 10; ++j) {
                m_blackMap->setPlatform(i, j);
            }
        }
        m_blackMap->setStartPosition(m_blackMap->convertToGlobalCoords(sf::Vector2i(5, 5)));

        GameMaps& maps = registry.ctx<GameMaps>();
        maps.whiteMap = m_whiteMap;
        maps.blackMap = m_blackMap;

        resetPlayers(registry, dispatcher);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        m_elapsedTime += dt;
        if(m_elapsedTime.asSeconds() > 31.0f) {
            //m_manager->setLevel(registry, dispatcher, std::make_shared<LevelOne>(m_manager));
        }
    }

private:
    sf::Time    m_elapsedTime;
};

#endif // LEVEL_H_INCLUDED
