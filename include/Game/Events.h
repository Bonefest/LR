#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

struct KeyEvent {
    explicit KeyEvent(sf::Keyboard::Key l_key, bool l_pressed): key(l_key), pressed(l_pressed) { }

    sf::Keyboard::Key   key;
    bool                pressed;
};

struct PlayerAttackEvent {

    explicit PlayerAttackEvent(entt::entity l_player): player(l_player) { }

    entt::entity player;
};


#endif // EVENTS_H_INCLUDED
