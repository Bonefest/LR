#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <string>

struct KeyEvent {
    explicit KeyEvent(sf::Keyboard::Key l_key, bool l_pressed): key(l_key), pressed(l_pressed) { }

    sf::Keyboard::Key   key;
    bool                pressed;
};

struct PlayerAttackEvent {

    explicit PlayerAttackEvent(entt::entity l_player): player(l_player) { }

    entt::entity player;
};

struct PlayerFallEvent {
    explicit PlayerFallEvent(entt::entity l_player): player(l_player) { }

    entt::entity player;
};

struct TipEvent {

    TipEvent(const std::string& l_message,
             const sf::Time& l_duration,
             int l_priority): message(l_message),
                              duration(l_duration),
                              priority(l_priority) { }

    std::string message;
    sf::Time    duration;
    int         priority;
};


#endif // EVENTS_H_INCLUDED
