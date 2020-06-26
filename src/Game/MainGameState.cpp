#include "Game/MainGameState.h"
#include <iostream>

#include "Game/Systems.h"

MainGameState::MainGameState(StateManager* manager): BaseState(manager) { }

void MainGameState::onCreate() {

    AnimationManager* animationManager = m_smanager->getContext()->animationManager;
    animationManager->loadAnimationsFromFile("animations.json");

    SystemsManager* manager = m_smanager->getContext()->systemsManager;
    manager->addSystem(std::make_shared<PlayerAnimationsControllSystem>(), 5);
    manager->addSystem(std::make_shared<KeyEventsNotifier>(), 5);

    auto windowSize = m_smanager->getContext()->window->getSize();

    m_leftCamera = sf::View(sf::Vector2f(-windowSize.x, 0.0f),
                            sf::Vector2f(windowSize.x * 0.5f,  windowSize.y));
    m_leftCamera.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f));

    m_rightCamera = sf::View(sf::Vector2f(windowSize.x , 0.0f),
                             sf::Vector2f(windowSize.x * 0.5f, windowSize.y));
    m_rightCamera.setViewport(sf::FloatRect(0.5f, 0.0f, 0.5f, 1.0f));

    m_view = sf::View(sf::Vector2f(windowSize.x, windowSize.y) * 0.5f,
                      sf::Vector2f(windowSize.x, windowSize.y));

    m_view.zoom(0.5f);

    createPlayer(BLACK);
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

}


entt::entity MainGameState::createPlayer(PlayerColor color) {
    entt::registry& registry = m_smanager->getContext()->systemsManager->getRegistry();
    entt::entity player = registry.create();

    AnimationManager* animManager = m_smanager->getContext()->animationManager;

    std::shared_ptr<AnimatedSprite> sprite = std::make_shared<AnimatedSprite>();
    sprite->setAnimation("idle",
                         animManager->getAnimation( (color == BLACK) ? "black_idle" : "white_idle"));
    sprite->setAnimation("move",
                         animManager->getAnimation( (color == BLACK) ? "black_move" : "white_move"));
    sprite->setOrigin(64.0f, 64.0f);

    if(color == BLACK) {
        sprite->setAnimation("move_through", animManager->getAnimation("black_move_through"));
    } else {
        sprite->setAnimation("attack", animManager->getAnimation("white_attack"));
    }

//    sprite->setAnimation("transform",
//                         animManager->getAnimation( (color == BLACK) ? "black_transform" : "white_transform"));

    PlayerStatePtr idleState = std::make_shared<PlayerIdleState>(player);
    registry.assign<Player>(player, sprite, idleState, color);

    idleState->onActivate(registry, m_smanager->getContext()->systemsManager->getDispatcher());

    return player;
}
