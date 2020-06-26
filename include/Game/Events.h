#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

struct KeyEvent {
    explicit KeyEvent(sf::Keyboard::Key l_key, bool l_pressed): key(l_key), pressed(l_pressed) { }

    sf::Keyboard::Key   key;
    bool                pressed;
};


#endif // EVENTS_H_INCLUDED
