#include "Window.h"

Window::Window(): m_title("Window"), m_windowSize(640, 480), m_fullscreen(false) {
    create();
    initBindings();
}

Window::Window(const std::string& title,
               sf::Vector2u windowSize,
               bool fullscreen): m_title(title),
                                 m_windowSize(windowSize), m_fullscreen(fullscreen) {

    create();
    initBindings();
}

Window::~Window() {
    destroy();
}

void Window::initBindings() {
    Binding l_closeBinding("System_Close");
    l_closeBinding.addEvent(Event(EventType::Closed));

    Binding l_fullscreenBinding("System_Fullscreen");
    l_fullscreenBinding.addEvent(Event(EventType::KeyPressed, sf::Keyboard::Enter));
    l_fullscreenBinding.addEvent(Event(EventType::Keyboard, sf::Keyboard::LAlt));

    m_eventManager.addBinding(l_closeBinding);
    m_eventManager.addBinding(l_fullscreenBinding);
    m_eventManager.addCallback(StateType::SYSTEM, "System_Close", &Window::closeEvent, this);
    m_eventManager.addCallback(StateType::SYSTEM, "System_Fullscreen", &Window::toggleFullscreenEvent, this);
}

void Window::closeEvent(EventDetails* details) {
    m_window.close();
}

void Window::toggleFullscreenEvent(EventDetails* details) {
    setFullscreen(!m_fullscreen);
}

void Window::update() {
    sf::Event event;
    while(m_window.pollEvent(event)) {
        m_eventManager.handleEvent(event);
    }
    m_eventManager.update();
}

void Window::setFullscreen(bool toggle) {
    m_fullscreen = toggle;
    destroy();
    create();
}

void Window::beganDrawing(const sf::Color& clearColor) {
    m_window.clear(clearColor);
}

void Window::draw(const sf::Drawable& drawable) {
    m_window.draw(drawable);
}

void Window::endDrawing() {
    m_window.display();
}

void Window::create() {
    auto style = isFullscreen() ? sf::Style::Fullscreen : sf::Style::Default;
    m_window.create({m_windowSize.x, m_windowSize.y, 32}, m_title, style);
}

void Window::destroy() {
    m_window.close();
}

bool Window::loadBindings(const std::string& fileName) {
    return m_eventManager.loadBindingsFromFile(fileName);
}
