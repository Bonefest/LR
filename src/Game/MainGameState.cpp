#include "Game/MainGameState.h"
#include <iostream>

#include "Game/Systems.h"
#include "Game/Components.h"

#include "Game/FlameState.h"

#include "Game/Level.h"

MainGameState::MainGameState(StateManager* manager): BaseState(manager) { }

void MainGameState::onCreate() {

    AnimationManager* animationManager = m_smanager->getContext()->animationManager;
    animationManager->loadAnimationsFromFile("animations.json");

    SystemsManager* manager = m_smanager->getContext()->systemsManager;


    manager->addSystem(std::make_shared<GameMapsRenderingSystem>(), 1);
    manager->addSystem(std::make_shared<FlameRenderingSystem>(), 2);
    manager->addSystem(std::make_shared<PlayerAnimationsControllSystem>(), 3);
    manager->addSystem(std::make_shared<KeyEventsNotifier>(), 3);
    manager->addSystem(std::make_shared<AttackSystem>(), 4);
    manager->addSystem(std::make_shared<UIRenderingSystem>(m_smanager->getContext()->textureManager), 5);
    manager->addSystem(std::make_shared<UITipSystem>(), 6);

    auto windowSize = m_smanager->getContext()->window->getSize();

    m_blackCamera = sf::View(sf::Vector2f(0.0f, 0.0f),
                            sf::Vector2f(windowSize.x * 0.5f,  windowSize.y));
    m_blackCamera.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f));
    m_blackCamera.zoom(0.5f);

    m_whiteCamera = sf::View(sf::Vector2f(0.0f , 0.0f),
                             sf::Vector2f(windowSize.x * 0.5f, windowSize.y));
    m_whiteCamera.setViewport(sf::FloatRect(0.5f, 0.0f, 0.5f, 1.0f));
    m_whiteCamera.zoom(0.5f);

    m_uiCamera = sf::View(sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f),
                          sf::Vector2f(windowSize.x, windowSize.y));

    m_uiCamera.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

    entt::registry& registry = manager->getRegistry();
    registry.set<GameData>(3);
    registry.set<CamerasContext>(&m_blackCamera, &m_whiteCamera, &m_uiCamera);

    m_black = createPlayer(BLACK);
    m_white = createPlayer(WHITE);

    registry.set<GameMaps>(nullptr, nullptr);

    m_levelManager.setAnimationManager(m_smanager->getContext()->animationManager);
    m_levelManager.setLevel(registry, manager->getDispatcher(), std::make_shared<TutorialLevel>(&m_levelManager));

}

void MainGameState::onDestroy() {
    delete m_blackMap;
    delete m_whiteMap;
}

void MainGameState::onActivate() {

}

void MainGameState::onDeactivate() {

}

void MainGameState::draw() {

}

void MainGameState::update(const sf::Time& dt) {

    SystemsManager* manager = m_smanager->getContext()->systemsManager;
    m_levelManager.update(manager->getRegistry(), manager->getDispatcher(), dt);

    Player& blackPlayer = m_smanager->getContext()->systemsManager->getRegistry().get<Player>(m_black);
    Player& whitePlayer = m_smanager->getContext()->systemsManager->getRegistry().get<Player>(m_white);

    GameMaps& maps = manager->getRegistry().ctx<GameMaps>();

    if(blackPlayer.gameMap == maps.blackMap) {
        m_blackCamera.setCenter(blackPlayer.sprite->getPosition());
        m_whiteCamera.setCenter(whitePlayer.sprite->getPosition());
    } else {
        m_blackCamera.setCenter(whitePlayer.sprite->getPosition());
        m_whiteCamera.setCenter(blackPlayer.sprite->getPosition());
    }
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
    sprite->setOrigin(64.0f, 100.0f);

    if(color == BLACK) {
        sprite->setAnimation("move_through", animManager->getAnimation("black_move_through"));
    } else {
        sprite->setAnimation("attack", animManager->getAnimation("white_attack"));
    }

    sprite->setAnimation("transform",
                         animManager->getAnimation( (color == BLACK) ? "black_transform" : "white_transform"));

    PlayerStatePtr idleState = std::make_shared<PlayerIdleState>(player);
    registry.assign<Player>(player, sprite, idleState, color, (color == BLACK) ? m_blackMap : m_whiteMap);

    idleState->onActivate(registry, m_smanager->getContext()->systemsManager->getDispatcher());

    return player;
}

