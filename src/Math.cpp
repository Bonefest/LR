#include "Math.h"

#include <cmath>

float squareDistance(sf::Vector2f from, sf::Vector2f to) {
    sf::Vector2f diff = to - from;
    return diff.x * diff.x + diff.y * diff.y;
}

float distance(sf::Vector2f A, sf::Vector2f B) {
    return std::sqrt(squareDistance(A, B));
}

float length(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}


float dot(sf::Vector2f vecA, sf::Vector2f vecB) {
    return vecA.x * vecB.x + vecA.y * vecB.y;
}

sf::Vector2f normalize(sf::Vector2f vector) {
    float l = length(vector);
    return vector * (1.0f / l);
}


sf::Vector2f reflect(sf::Vector2f vector, sf::Vector2f normal) {
    normal = normalize(normal);
    return vector - 2.0f * normal * dot(vector, normal);
}

sf::Vector2f generateRandomDirection() {
    return normalize(sf::Vector2f(drand48() - drand48(), drand48() - drand48()));
}
