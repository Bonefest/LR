#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "AnimatedSprite.h"
#include "PlayerState.h"

#include <memory>

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


#endif // PLAYER_H_INCLUDED
