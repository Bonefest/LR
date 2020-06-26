#ifndef MAINGAME_H_INCLUDED
#define MAINGAME_H_INCLUDED

#include "Game.h"

class MainGame: public Game {
public:
    MainGame(const std::string& title, sf::Vector2u windowSize);

    virtual void onCreate();

    virtual void update(const sf::Time& dt);
    virtual void draw();

};

#endif // MAINGAME_H_INCLUDED
