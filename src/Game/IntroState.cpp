#include "Game/IntroState.h"
#include "Game/MainGameState.h"

#include "StateType.h"
#include <iostream>


IntroState::IntroState(StateManager* manager): BaseState(manager) { }

void IntroState::onCreate() {

}

void IntroState::onDestroy() {

}

void IntroState::onActivate() {

}

void IntroState::onDeactivate() {

}

void IntroState::draw() {

}

void IntroState::update(const sf::Time& dt) {
    //switch to main game
    std::cout << "State\n";
    m_smanager->setNextState(StateType::Game);

}
