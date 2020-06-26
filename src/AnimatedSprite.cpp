#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(): m_currentAnimation(nullptr), m_frozen(false), m_flipped(false) { }

void AnimatedSprite::setAnimation(const string& animationName, AnimationPtr animation) {
    m_animations[animationName] = animation;
    if(m_currentAnimation == nullptr) {
        activateAnimation(animationName);
    }
}

bool AnimatedSprite::activateAnimation(const string& animationName) {
    auto animIt = m_animations.find(animationName);
    if(animIt != m_animations.end()) {
        m_currentAnimation = animIt->second;
        setTexture(*m_currentAnimation->getTexture());
        updateTextureRect();
        return true;
    }

    return false;
}

bool AnimatedSprite::hasAnimation(const string& animationName) const {
    return m_animations.find(animationName) != m_animations.end();
}

void AnimatedSprite::update(const sf::Time& dt) {
    if(m_currentAnimation && !m_frozen) {
        m_currentAnimation->update(dt);
        updateTextureRect();
    }
}

AnimationPtr AnimatedSprite::getCurrentAnimation() {
    return m_currentAnimation;
}

void AnimatedSprite::setFlipped(bool flipped) {
    m_flipped = flipped;
}

bool AnimatedSprite::isFlipped() const {
    return m_flipped;
}

void AnimatedSprite::freeze(bool freeze) {
    m_frozen = freeze;
}

void AnimatedSprite::resetAnimation() {
    if(m_currentAnimation != nullptr) {
        m_currentAnimation->reset();
        updateTextureRect();
    }
}

void AnimatedSprite::updateTextureRect() {
    if(m_currentAnimation != nullptr) {
        auto frame = m_currentAnimation->getCurrentFrame();
        if(m_flipped) {
            frame.left = frame.left + frame.width;
            frame.width = -frame.width;
        }
        setTextureRect(frame);
    }
}
