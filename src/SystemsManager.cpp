#include "SystemsManager.h"

using std::shared_ptr;
using std::pair;

#include <list>

entt::registry& SystemsManager::getRegistry() {
    return m_registry;
}

entt::dispatcher& SystemsManager::getDispatcher() {
    return m_dispatcher;
}

void SystemsManager::updateSystems(const sf::Time& delta) {
    std::list<prioritized_system> poppedSystems;

    while(!m_systems.empty()) {
        poppedSystems.push_front(m_systems.top());
        m_systems.pop();
    }

    for(auto system: poppedSystems) {

        system.second->update(m_registry, m_dispatcher, delta);
        m_systems.push(system);
    }
}

void SystemsManager::addSystem(SystemPtr system, int priority) {
    m_systems.push(prioritized_system{priority, system});
    system->onCreate(m_registry, m_dispatcher);
}
