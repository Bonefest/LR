#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

#include "EventManager.h"
#include "SystemsManager.h"

class SystemsManager;

class Window {
public:
    Window();
    Window(const std::string& title, sf::Vector2u windowSize, bool fullscreen = false);

    ~Window();

    void update();

    void beganDrawing(const sf::Color& clearColor = sf::Color::Black);
    void draw(const sf::Drawable& drawable);
    void endDrawing();

    inline sf::Vector2u getSize() const { return m_windowSize; }
    inline std::string getTitle() const { return m_title; }
    inline EventManager* getEventManager() { return &m_eventManager; }
    inline SystemsManager* getSystemsManager() { return m_systemsManager; }

    inline bool isFullscreen() const { return m_fullscreen; }
    void setFullscreen(bool toggle);

    inline bool isOpen() const { return m_window.isOpen(); }

    void closeEvent(EventDetails* details);
    void toggleFullscreenEvent(EventDetails* details);

    bool loadBindings(const std::string& fileName);

    inline sf::RenderWindow* getWindow() { return &m_window; }

    void setView(const sf::View& view);

private:
    void create();
    void destroy();

    void initBindings();

    sf::RenderWindow    m_window;
    EventManager        m_eventManager;
    SystemsManager*     m_systemsManager;

    std::string         m_title;
    sf::Vector2u        m_windowSize;
    bool                m_fullscreen;


};


#endif // WINDOW_H_INCLUDED
