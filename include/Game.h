#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Window.h"
#include "StateManager.h"
#include "ResourcesManager.h"
#include "AnimationManager.h"

class Game {
public:
    Game(const std::string& title, sf::Vector2u windowSize);

    void run();

    virtual void onCreate() { }

    virtual void handleInput() { }
    virtual void update(const sf::Time& dt) { }
    virtual void draw() { }

    void setFrameLimit(sf::Uint32 framesPerSec);
    inline sf::Uint32 getFrameLimit() const { return m_framesPerSec; }


protected:
    Window              m_window;
    TextureManager      m_texManager;
    StateManager        m_stateManager;
    AnimationManager*   m_animationManager;

private:
    sf::Clock   m_clock;
    sf::Time    m_elapsedTime;

    sf::Time    m_frameTime;
    sf::Uint32  m_framesPerSec;
};

#endif // GAME_H_INCLUDED
