#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <SFML/Graphics.hpp>

namespace Constants {
    const static sf::Vector2u mapSize = sf::Vector2u(100, 100);
    const static sf::Vector2f platformSize = sf::Vector2f(100.0f, 100.0f);

    const static sf::Vector2f i = sf::Vector2f(1.0f, 0.0f);
    const static sf::Vector2f j = sf::Vector2f(0.707f, -0.707f);
    const static sf::Vector2f k = sf::Vector2f(0.0f, 1.0f);
}


enum MovingDirection {
    LEFT, TOP, RIGHT, BOTTOM, NONE
};

#endif // CONSTANTS_H_INCLUDED
