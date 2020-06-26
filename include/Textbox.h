#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include <deque>
#include <string>

#include <SFML/Graphics.hpp>
#include "Window.h"

using MessageQueue = std::deque<std::string>;

class Textbox {
public:

    Textbox();
    Textbox(sf::Uint8 lines, unsigned charSize, sf::Uint32 width, sf::Vector2f position);
    void init(sf::Uint8 lines, unsigned charSize, sf::Uint32 width, sf::Vector2f position);

    void addMessage(const std::string& message);
    void clear();

    void draw(Window& window);

    void setPosition(sf::Vector2f position);
    inline sf::Vector2f getPosition() const { return m_position; }

    inline sf::Vector2f getSize() const { return m_background.getSize(); }

private:
    void calculateTextContent();

    sf::Uint8           m_maxQueueSize;
    MessageQueue        m_messages;
    sf::Font            m_font;
    sf::Text            m_text;
    sf::RectangleShape  m_background;

    sf::Vector2f        m_position;
};

#endif // TEXTBOX_H_INCLUDED
