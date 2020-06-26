#include "Game.h"
#include <iostream>

Game::Game(const std::string& title, sf::Vector2u windowSize): m_window(title, windowSize) {
    setFrameLimit(60);

    StateContext* context = m_stateManager.getContext();

    m_animationManager = new AnimationManager(&m_texManager);

    context->window = &m_window;
    context->eventManager = m_window.getEventManager();
    context->animationManager = m_animationManager;
    context->systemsManager = m_window.getSystemsManager();
    context->textureManager = &m_texManager;
}

Game::~Game() {
    delete m_animationManager;
}

void Game::run() {

    onCreate();

    while(m_window.isOpen()) {

        m_window.update();
        handleInput();

        m_elapsedTime += m_clock.restart();
        if(m_elapsedTime > m_frameTime) {
            update(m_frameTime);
            m_stateManager.getContext()->systemsManager->updateSystems(m_frameTime);
            m_elapsedTime -= m_frameTime;
        }

        m_window.beganDrawing(sf::Color::White);
        draw();
        m_stateManager.getContext()->systemsManager->drawSystems(m_window);
        m_window.endDrawing();
    }
}

void Game::setFrameLimit(sf::Uint32 framesPerSec) {
    if(framesPerSec > 0) {
        m_frameTime = sf::seconds(1.0f / float(framesPerSec));
    } else {
        m_frameTime = sf::seconds(0.016f);
    }

    m_framesPerSec = framesPerSec;
}


