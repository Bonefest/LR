#ifndef SYSTEMSMANAGER_H_INCLUDED
#define SYSTEMSMANAGER_H_INCLUDED

#include <queue>
#include <memory>

#include "System.h"
#include <SFML/Graphics.hpp>

typedef std::pair<int, SystemPtr> prioritized_system;

class SystemsManager {
public:

    void updateSystems(const sf::Time& dt);

    void addSystem(SystemPtr system, int priority = 0);

    entt::registry& getRegistry();
    entt::dispatcher& getDispatcher();

private:
    entt::registry m_registry;
    entt::dispatcher m_dispatcher;

    std::priority_queue<prioritized_system> m_systems;
};


#endif // SYSTEMSMANAGER_H_INCLUDED
