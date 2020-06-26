#include "Textbox.h"

Textbox::Textbox() {
    init(3, 8, 250, sf::Vector2f(0.0f, 0.0f));
}

Textbox::Textbox(sf::Uint8 lines, unsigned charSize, sf::Uint32 width, sf::Vector2f position) {
    init(lines, charSize, width, position);
}

void Textbox::init(sf::Uint8 lines, unsigned charSize, sf::Uint32 width, sf::Vector2f position) {
    m_position = position;

    m_maxQueueSize = std::max<sf::Uint8>(lines, 1);
    m_messages.clear();

    m_font.loadFromFile("arial.ttf");

    m_text.setCharacterSize(charSize);
    m_text.setFont(m_font);
    m_text.setString("");
    m_text.setPosition(position + sf::Vector2f(5.0f, 5.0f));

    m_background.setFillColor(sf::Color(128, 128, 128, 90));
    m_background.setPosition(position);
    m_background.setSize(sf::Vector2f(width, lines * charSize * 1.2f));
}

void Textbox::addMessage(const std::string& message) {
    m_messages.push_back(message);
    if(m_messages.size() > m_maxQueueSize) {
        m_messages.pop_front();
    }

    calculateTextContent();
}

void Textbox::calculateTextContent() {
    std::string result = "";
    for(auto line : m_messages) {
        result = result + line + "\n";
    }

    m_text.setString(result);
}

void Textbox::clear() {
    m_messages.clear();
}

void Textbox::draw(Window& window) {
    window.draw(m_background);
    window.draw(m_text);
}

void Textbox::setPosition(sf::Vector2f position) {
    m_text.setPosition(position + sf::Vector2f(5.0f, 5.0f));
    m_background.setPosition(position);
}
