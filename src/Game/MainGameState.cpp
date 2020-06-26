#include "Game/MainGameState.h"
#include <iostream>

MainGameState::MainGameState(StateManager* manager): BaseState(manager) { }

void MainGameState::onCreate() {

}

void MainGameState::onDestroy() {

}

void MainGameState::onActivate() {

}

void MainGameState::onDeactivate() {

}

void MainGameState::draw() {

}

void MainGameState::update(const sf::Time& dt) {
    std::cout << "MainGame\n";
}
