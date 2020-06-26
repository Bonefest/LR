#ifndef ANIMATEDSPRITE_H_INCLUDED
#define ANIMATEDSPRITE_H_INCLUDED

#include <map>
#include <string>

#include "Animation.h"

using std::map;
using std::string;

class AnimatedSprite: public sf::Sprite {
public:
    AnimatedSprite();

    void setAnimation(const string& animationName, AnimationPtr animation);
    bool activateAnimation(const string& animationName);

    bool hasAnimation(const string& animationName) const;

    void update(const sf::Time& dt);

    AnimationPtr getCurrentAnimation();

    void setFlipped(bool flipped);
    bool isFlipped() const;

    void freeze(bool freeze);
    void resetAnimation();

    inline bool isFrozen() const { return m_frozen; }

private:
    void updateTextureRect();

    map<string, AnimationPtr>       m_animations;
    AnimationPtr                    m_currentAnimation;

    bool                            m_frozen;
    bool                            m_flipped;
};

#endif // ANIMATEDSPRITE_H_INCLUDED
