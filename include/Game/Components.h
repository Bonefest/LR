#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "AnimatedSprite.h"
#include "PlayerState.h"

#include <memory>

#include "Constants.h"

enum PlayerColor {
    BLACK, WHITE
};

struct Player {

    explicit Player(std::shared_ptr<AnimatedSprite> l_sprite,
                    PlayerStatePtr state,
                    PlayerColor l_color): sprite(l_sprite),
                                          currentState(state),
                                          color(l_color) { }

    std::shared_ptr<AnimatedSprite> sprite;
    PlayerStatePtr                  currentState;
    PlayerColor                     color;
};

enum class Side { LEFT, TOP, RIGHT, DOWN };

struct Obstacle {

    Obstacle(PlayerColor l_color,
             sf::Vector2f l_position, sf::Vector3i l_size): color(l_color), position(l_position), size(l_size) {

        sf::Color topColor = (color == WHITE) ? sf::Color(0, 0, 0, 255)      : sf::Color(255, 255, 255, 255);
        sf::Color frontColor = (color == WHITE) ? sf::Color(32, 32, 32, 255) : sf::Color(223, 223, 233, 255);
        sf::Color rightColor = (color == WHITE) ? sf::Color(48, 48, 48, 255) : sf::Color(207, 207, 207, 255);

        // Top
        vertices[0].position = Constants::i * (position.x - size.x * 0.5f) + Constants::j * (position.y - size.y * 0.5f);
        vertices[1].position = Constants::i * (position.x - size.x * 0.5f) + Constants::j * (position.y + size.y * 0.5f);
        vertices[2].position = Constants::i * (position.x + size.x * 0.5f) + Constants::j * (position.y + size.y * 0.5f);

        vertices[3].position = Constants::i * (position.x + size.x * 0.5f) + Constants::j * (position.y + size.y * 0.5f);
        vertices[4].position = Constants::i * (position.x + size.x * 0.5f) + Constants::j * (position.y - size.y * 0.5f);
        vertices[5].position = Constants::i * (position.x - size.x * 0.5f) + Constants::j * (position.y - size.y * 0.5f);

        // Front
        vertices[6].position = vertices[5].position;
        vertices[7].position = vertices[4].position;
        vertices[8].position = vertices[4].position + Constants::k * float(size.z);

        vertices[9].position  = vertices[8].position;
        vertices[10].position = vertices[0].position + Constants::k * float(size.z);
        vertices[11].position = vertices[0].position;

        // Right
        vertices[12].position = vertices[4].position;
        vertices[13].position = vertices[3].position;
        vertices[14].position = vertices[3].position + Constants::k * float(size.z);

        vertices[15].position = vertices[14].position;
        vertices[16].position = vertices[12].position + Constants::k * float(size.z);
        vertices[17].position = vertices[12].position;

        for(int i = 0;i < 18; ++i) {
            if(i < 6) {
                vertices[i].color = topColor;
            } else if(i < 12) {
                vertices[i].color = frontColor;
            } else {
                vertices[i].color = rightColor;
            }
        }

    }

    PlayerColor     color;

    sf::Vector2f    position;
    sf::Vector3i    size;

    sf::Vertex      vertices[18];
};

struct CamerasContext {
    CamerasContext(sf::View* l_blackView, sf::View* l_whiteView): blackView(l_blackView),
                                                                  whiteView(l_whiteView) { }

    sf::View*   blackView;
    sf::View*   whiteView;
};

#endif // COMPONENTS_H_INCLUDED
