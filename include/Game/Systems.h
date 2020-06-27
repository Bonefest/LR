#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "System.h"
#include "Components.h"
#include "Game/Events.h"

#include "Game/FlameState.h"

class PlayerAnimationsControllSystem: public System {
public:
    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        auto playersView = registry.view<Player>();
        playersView.each([&](entt::entity plr, Player& player) {
            player.currentState->update(registry, dispatcher, dt);
            player.sprite->update(dt);
        });
    }

    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        auto playersView = registry.view<Player>();
        CamerasContext& cameras = registry.ctx<CamerasContext>();
        GameMaps& gameMaps = registry.ctx<GameMaps>();
        playersView.each([&](entt::entity plr, Player& player){
            if(player.gameMap == gameMaps.whiteMap) {
                window.setView(*cameras.whiteView);
            }
            else {
                window.setView(*cameras.blackView);
            }

            window.draw(*player.sprite);

        });
    }
};

class KeyEventsNotifier: public System {
public:
    void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.sink<KeyEvent>().connect<&KeyEventsNotifier::onKeyEvent>(*this);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        auto playersView = registry.view<Player>();
        playersView.each([&](entt::entity plr, Player& player){
            for(auto key: m_keys) {
                player.currentState->onKeyEvent(key);
            }
        });

        m_keys.clear();
    }

    void onKeyEvent(const KeyEvent& event) {
        m_keys.push_back(event);
    }

private:
    std::vector<KeyEvent> m_keys;
};

class GameMapsRenderingSystem: public System {
public:
    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        CamerasContext& context = registry.ctx<CamerasContext>();
        GameMaps& gameMaps = registry.ctx<GameMaps>();

        window.setView(*context.whiteView);
        gameMaps.whiteMap->draw(window);

        window.setView(*context.blackView);
        gameMaps.blackMap->draw(window);
    }
};

class FlameRenderingSystem: public System {
public:

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        auto flamesView = registry.view<Flame>();
        flamesView.each([&](entt::entity flm, Flame& flame){
            flame.sprite->update(dt);
            flame.state->update(registry, dispatcher, dt);
        });
    }

    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        auto flamesView = registry.view<Flame>();
        CamerasContext cameras = registry.ctx<CamerasContext>();
        flamesView.each([&](entt::entity flm, Flame& flame){
            if(flame.color == BLACK) {
                window.setView(*cameras.blackView);
            } else {
                window.setView(*cameras.whiteView);
            }

            window.draw(*flame.sprite);
        });
    }
};
#include <iostream>
class AttackSystem: public System {
public:

    void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.sink<PlayerAttackEvent>().connect<&AttackSystem::onPlayerAttackEvent>(*this);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        GameMaps& gameMaps = registry.ctx<GameMaps>();
        for(auto event: m_events) {
            Player& player = registry.get<Player>(event.player);
            sf::Vector2f playerDirection = player.sprite->isFlipped() ? sf::Vector2f(-1.0f, 0.0f) : sf::Vector2f(1.0f, 0.0f);
            auto flamesView = registry.view<Flame>();

            PlayerColor actualColor = WHITE;
            if(player.gameMap == gameMaps.blackMap) {
                actualColor = BLACK;
            }


            flamesView.each([&](entt::entity flm, Flame& flame){
                if(flame.color == actualColor) {
                    sf::Vector2f relativePos = flame.sprite->getPosition() - player.sprite->getPosition();
                    float length = std::sqrt(relativePos.x * relativePos.x + relativePos.y * relativePos.y);
                    if(length > 0.001f) {

                        // Normalize
                        relativePos.x /= length;
                        relativePos.y /= length;
                        // Dot product
                        if(length < 45.0f && playerDirection.x * relativePos.x + playerDirection.y * relativePos.y > 0.0f) {
                            flame.state = std::make_shared<FlameDeathState>(flm);
                            return;
                        }
                    }
                }
            });


        }

        m_events.clear();
    }

    void onPlayerAttackEvent(const PlayerAttackEvent& event) {
        m_events.push_back(event);
    }

private:
    std::vector<PlayerAttackEvent> m_events;

};

class UIRenderingSystem: public System {
public:
    UIRenderingSystem(TextureManager* texManager): m_texManager(texManager) { }

    void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) {
        for(int i = 0;i < 3; ++i) {
            createHeart();
        }

        dispatcher.sink<PlayerFallEvent>().connect<&UIRenderingSystem::onPlayerFall>(*this);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {

        GameData& data = registry.ctx<GameData>();
        if(m_hearts.size() < data.health) {
            for(int i = 0;i < data.health - m_hearts.size(); ++i) {
                createHeart();
            }
        }

        data.health = std::max<int>(0, data.health - m_events.size());
        while(!m_events.empty()) {
            if(!m_hearts.empty()) {
                m_hearts.pop_back();
            }
            m_events.pop_back();
        }
    }

    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {

        CamerasContext& cameras = registry.ctx<CamerasContext>();
        auto windowSize = window.getSize();

        window.setView(*cameras.uiView);

        if(!m_hearts.empty()) {

            auto heartSize = m_hearts[0]->getTexture()->getSize();
            float totalWidth = heartSize.x * ( m_hearts.size() - 1);

            for(auto i = 0u;i < m_hearts.size(); ++i) {
                m_hearts[i]->setPosition(sf::Vector2f(windowSize.x * 0.5f - totalWidth * 0.5f + heartSize.x * i, windowSize.y * 0.1f));
                window.draw(*m_hearts[i]);
            }

        }
    }

    void onPlayerFall(const PlayerFallEvent& event) {
        m_events.push_back(event);
    }

private:
    void createHeart() {
        std::shared_ptr<sf::Sprite> sprite = std::make_shared<sf::Sprite>();
        sprite->setTexture(*m_texManager->get("heart"));

        auto texSize = sprite->getTexture()->getSize();
        sprite->setOrigin(texSize.x * 0.5f, texSize.y * 0.5f);

        m_hearts.push_back(sprite);
    }

    std::vector<std::shared_ptr<sf::Sprite>> m_hearts;
    TextureManager*                          m_texManager;

    std::vector<PlayerFallEvent>             m_events;
};

class UITipSystem: public System {
public:

    UITipSystem():m_currentMessage("", sf::seconds(9999.0f), -1) { }

    void onCreate(entt::registry& registry, entt::dispatcher& dispatcher) {
        dispatcher.sink<TipEvent>().connect<&UITipSystem::onTipEvent>(*this);

        m_textFont.loadFromFile("Resources/mago1.ttf");
        m_text.setFont(m_textFont);
        m_text.setCharacterSize(48);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, const sf::Time& dt) {
        m_elapsedTime += dt;
        if(m_elapsedTime > m_currentMessage.duration) {
            if(m_messages.empty()) {
                m_currentMessage.duration = sf::seconds(99999.0f);
                m_currentMessage.message = "";
                m_currentMessage.priority = -1;
            } else {
                m_currentMessage = m_messages.back();;
                m_messages.pop_back();
            }

            updateText();

            m_elapsedTime = sf::seconds(0.0f);
        }

        if(m_currentMessage.duration.asSeconds() > 0.001f) {
            float t = 0.0f;
            if(m_elapsedTime.asSeconds() < m_currentMessage.duration.asSeconds() * 0.4f) {
                t = std::min(m_elapsedTime.asSeconds() / (m_currentMessage.duration.asSeconds() * 0.4f), 1.0f);
            }
            else if(m_elapsedTime.asSeconds() < m_currentMessage.duration.asSeconds() * 0.8f) {
                t = 1.0f;
            } else {
                t = 1.0f - std::min<float>( (m_elapsedTime.asSeconds() - m_currentMessage.duration.asSeconds() * 0.8f) / (m_currentMessage.duration.asSeconds() * 0.2f), 1.0f);
            }

            m_text.setFillColor(sf::Color(128, 128, 128, t * 255));
        }
    }

    void draw(Window& window, entt::registry& registry, entt::dispatcher& dispatcher) {
        CamerasContext& context = registry.ctx<CamerasContext>();

        auto windowSize = window.getSize();
        m_text.setPosition(sf::Vector2f(windowSize.x, windowSize.y) * 0.5f);
        window.setView(*context.uiView);
        window.draw(m_text);
    }

    void onTipEvent(const TipEvent& event) {
        if(m_currentMessage.priority < event.priority) {
            m_currentMessage = event;
            updateText();
            m_messages.clear();
        } else {

            if(!m_messages.empty()) {
                auto it = m_messages.begin();
                for(auto it = m_messages.begin(); it != m_messages.end(); it++) {
                    if(it->priority < event.priority) {

                        break;
                    }
                }

                m_messages.insert(it, event);
                it = it++;
                while(it != m_messages.end()) {
                    it = m_messages.erase(it);
                }
            } else {
                m_messages.push_back(event);
            }

        }
    }

private:
    void updateText() {
        m_text.setString(m_currentMessage.message);
        m_text.setFillColor(sf::Color(128, 128, 128, 255));

        auto textSize = m_text.getLocalBounds();
        m_text.setOrigin(textSize.width * 0.5f, textSize.height * 0.5f);

    }

    std::list<TipEvent>         m_messages;
    TipEvent                    m_currentMessage;
    sf::Font                    m_textFont;
    sf::Text                    m_text;
    sf::Time                    m_elapsedTime;
};

#endif // SYSTEMS_H_INCLUDED
