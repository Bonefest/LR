#include "Game/MainGame.h"
#include "Game/IntroState.h"
#include "Game/MainGameState.h"

MainGame::MainGame(const std::string& title,
                   sf::Vector2u windowSize): Game(title, windowSize) { }


void MainGame::onCreate() {
    m_stateManager.getContext()->textureManager->loadPathsFromFile("textures.cfg");

    m_stateManager.registerState<IntroState>(StateType::Intro);
    m_stateManager.registerState<MainGameState>(StateType::Game);
    m_stateManager.setState(StateType::Intro);
}

void MainGame::draw() {
    m_stateManager.draw();
}

void MainGame::update(const sf::Time& dt) {
    m_stateManager.update(dt);
}
