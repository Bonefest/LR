#include "Game/MainGameState.h"
#include <iostream>

#include "Game/Systems.h"
#include "Game/Components.h"

#include "Game/FlameState.h"

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

    manager->getDispatcher().trigger<TipEvent>("          This world is broken.\n"
                                               "It seems like you know now how to connect\n"
                                               "          to parallel university.", sf::seconds(4.0f), 5);
    manager->getDispatcher().trigger<TipEvent>("Use [WASD] to move around", sf::seconds(4.0f),5);
    manager->getDispatcher().trigger<TipEvent>("   Wait! All your actions are\n"
                                               "connected to parallel university.\n", sf::seconds(3.0f), 5);
    manager->getDispatcher().trigger<TipEvent>("Be sure you know what you are doing...\n", sf::seconds(3.0f), 5);
    manager->getDispatcher().trigger<TipEvent>("Use [X] for sprint and [Z] for attacking\n", sf::seconds(4.0f), 5);
    manager->getDispatcher().trigger<TipEvent>("Use [Space] for swapping between universities\n", sf::seconds(4.0f), 5);

    entt::registry& registry = manager->getRegistry();
    registry.set<GameData>(3);
    registry.set<CamerasContext>(&m_blackCamera, &m_whiteCamera, &m_uiCamera);

    m_blackMap = new GameMap(BLACK);
    m_blackMap->setStartPosition(sf::Vector2f(0.0f, 0.0f));
    m_whiteMap = new GameMap(WHITE);
    m_whiteMap->setStartPosition(sf::Vector2f(0.0f, 0.0f));

    m_black = createPlayer(BLACK);
    m_white = createPlayer(WHITE);

    registry.set<GameMaps>(m_blackMap, m_whiteMap);

    m_blackMap->setPlatform(0, 0);
    m_blackMap->setPlatform(1, 0);
    m_blackMap->setPlatform(1, 1);
    m_blackMap->setPlatform(2, 0);
    m_blackMap->setPlatform(4, 0);
    m_blackMap->setPlatform(6, 0);
    m_blackMap->setPlatform(0, 2);
    m_blackMap->setPlatform(2, 2);
    m_blackMap->setPlatform(4, 2);
    m_blackMap->setPlatform(6, 2);

    m_whiteMap->setPlatform(0, 0);
    m_whiteMap->setPlatform(1, 0);
    m_whiteMap->setPlatform(1, 1);
    m_whiteMap->setPlatform(1, 3);

    createFlame(m_blackMap->convertToGlobalCoords(sf::Vector2i(6, 0)), BLACK);

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

    Player& blackPlayer = m_smanager->getContext()->systemsManager->getRegistry().get<Player>(m_black);
    Player& whitePlayer = m_smanager->getContext()->systemsManager->getRegistry().get<Player>(m_white);

    if(blackPlayer.gameMap == m_blackMap) {
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

entt::entity MainGameState::createFlame(const sf::Vector2f& position, PlayerColor color) {
    entt::registry& registry = m_smanager->getContext()->systemsManager->getRegistry();
    entt::entity flame = registry.create();


    std::shared_ptr<AnimatedSprite> sprite = std::make_shared<AnimatedSprite>();
    sprite->setAnimation("flame", m_smanager->getContext()->animationManager->getAnimation("flame"));
    sprite->activateAnimation("flame");
    sprite->setPosition(position);
    sprite->setOrigin(50.0f, 50.0f);

    PlayerStatePtr state = std::make_shared<FlameIdleState>(flame);

    registry.assign<Flame>(flame, sprite, color, state);

    return flame;
}
