#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <SFML/Graphics.hpp>

float squareDistance(sf::Vector2f from, sf::Vector2f to);
float distance(sf::Vector2f from, sf::Vector2f to);
float length(sf::Vector2f vector);

float dot(sf::Vector2f vecA, sf::Vector2f vecB);
sf::Vector2f normalize(sf::Vector2f vector);
sf::Vector2f reflect(sf::Vector2f vector, sf::Vector2f normal);

sf::Vector2f generateRandomDirection();


#endif // MATH_H_INCLUDED
