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

    void resetPlayers(entt::registry& registry, entt::dispatcher& dispatcher);

    entt::entity createFlame(entt::registry& registry, const sf::Vector2f& position, PlayerColor color);

protected:
    GameMap*        m_whiteMap;
    GameMap*        m_blackMap;

    LevelManager*   m_manager;
};

class LevelOne: public Level {
public:
    LevelOne(LevelManager* manager): Level(manager) { }

    void onLoad(entt::registry& registry, entt::dispatcher& dispatcher) {

        dispatcher.trigger<TipEvent>("Level one", sf::seconds(5.0f), 100);
        std::cout << "here\n";

        m_whiteMap = new GameMap(WHITE);
        m_blackMap = new GameMap(BLACK);

        m_whiteMap->setPlatform(5, 5);
        m_whiteMap->setPlatform(5, 6);
        m_whiteMap->setPlatform(5, 7);
        m_whiteMap->setPlatform(5, 8);
        m_whiteMap->setPlatform(4, 8);
        m_whiteMap->setPlatform(3, 8);
        m_whiteMap->setPlatform(6, 8);
        m_whiteMap->setPlatform(7, 8);
        m_whiteMap->setPlatform(8, 8);
        m_whiteMap->setPlatform(8, 9);
        m_whiteMap->setPlatform(8, 10);
        m_whiteMap->setPlatform(8, 11);
        m_whiteMap->setPlatform(8, 12);
        m_whiteMap->setPlatform(7, 12);
        m_whiteMap->setPlatform(6, 12);
        m_whiteMap->setPlatform(5, 12);
        m_whiteMap->setPlatform(3, 12);
        m_whiteMap->setPlatform(2, 12);


        m_whiteMap->setStartPosition(m_whiteMap->convertToGlobalCoords(sf::Vector2i(5, 5)));


        m_blackMap->setPlatform(10, 12);
        m_blackMap->setPlatform(10, 11);
        m_blackMap->setPlatform(10, 10);
        m_blackMap->setPlatform(10, 9);
        m_blackMap->setPlatform(11, 9);
        m_blackMap->setPlatform(12, 9);
        m_blackMap->setPlatform(9, 9);
        m_blackMap->setPlatform(8, 9);
        m_blackMap->setPlatform(7, 9);
        m_blackMap->setPlatform(7, 8);
        m_blackMap->setPlatform(7, 7);
        m_blackMap->setPlatform(7, 6);
        m_blackMap->setPlatform(7, 5);
        m_blackMap->setPlatform(8, 5);
        m_blackMap->setPlatform(9, 5);
        m_blackMap->setPlatform(10, 5);
        m_blackMap->setPlatform(11, 5);
        m_blackMap->setPlatform(12, 5);

        m_blackMap->setStartPosition(m_blackMap->convertToGlobalCoords(sf::Vector2i(10, 12)));

        GameMaps& maps = registry.ctx<GameMaps>();
        maps.whiteMap = m_whiteMap;
        maps.blackMap = m_blackMap;

        resetPlayers(registry, dispatcher);

        m_flame[0] = createFlame(registry, m_whiteMap->convertToGlobalCoords(sf::Vector2i(2, 12)), WHITE);
        m_flame[1] = createFlame(registry, m_whiteMap->convertToGlobalCoords(sf::Vector2i(12, 5)), BLACK);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        if(!registry.valid(m_flame[0]) && !registry.valid(m_flame[1])) {
            std::cout << "switch to ... \n";
        }
    }

private:
    entt::entity m_flame[2];

};

class TutorialLevel: public Level {
public:
    TutorialLevel(LevelManager* manager): Level(manager) { }

    virtual void onLoad(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.trigger<TipEvent>("[Tutorial] (to skip press R)", sf::seconds(2.0f), 5);
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
            m_manager->setLevel(registry, dispatcher, std::make_shared<LevelOne>(m_manager));
        }
    }

private:
    sf::Time    m_elapsedTime;
};

#endif // LEVEL_H_INCLUDED
