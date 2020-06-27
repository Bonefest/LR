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
                                    gameMap(gmap),
                                    falling(false) { }

    std::shared_ptr<AnimatedSprite> sprite;
    PlayerStatePtr                  currentState;
    PlayerColor                     color;
    GameMap*                        gameMap;
    MovingDirection                 lastMovingDirection;
    sf::Vector2f                    lastPosition;
    sf::Time                        idle;
    bool                            falling;
    bool                            dead;
};

struct CamerasContext {
    CamerasContext(sf::View* l_blackView,
                   sf::View* l_whiteView,
                   sf::View* l_uiView): blackView(l_blackView),
                                        whiteView(l_whiteView),
                                        uiView(l_uiView) { }

    sf::View*   blackView;
    sf::View*   whiteView;
    sf::View*   uiView;
};

struct Flame {

    explicit Flame(std::shared_ptr<AnimatedSprite> l_sprite,
                   PlayerColor l_color,
                   PlayerStatePtr l_state): sprite(l_sprite),
                                            color(l_color),
                                            state(l_state) { }

    std::shared_ptr<AnimatedSprite> sprite;
    PlayerColor                     color;
    PlayerStatePtr                  state;
};

struct GameData {
    GameData(int l_health): health(l_health) { }

    int health;
};

#endif // COMPONENTS_H_INCLUDED
