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

#include "Game/GameMap.h"

class GameMap;

struct Player {

    explicit Player(std::shared_ptr<AnimatedSprite> l_sprite,
                    PlayerStatePtr state,
                    PlayerColor l_color,
                    GameMap* gmap): sprite(l_sprite),
                                    currentState(state),
                                    color(l_color),
                                    gameMap(gmap) { }

    std::shared_ptr<AnimatedSprite> sprite;
    PlayerStatePtr                  currentState;
    PlayerColor                     color;
    GameMap*                        gameMap;
    MovingDirection                 lastMovingDirection;
    sf::Vector2f                    lastPosition;
    sf::Time                        idle;
};

struct CamerasContext {
    CamerasContext(sf::View* l_blackView, sf::View* l_whiteView): blackView(l_blackView),
                                                                  whiteView(l_whiteView) { }

    sf::View*   blackView;
    sf::View*   whiteView;
};

#endif // COMPONENTS_H_INCLUDED
