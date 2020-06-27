#include "Game/GameMap.h"
#include "Game/Constants.h"

GameMap::GameMap(PlayerColor color): m_color(color), m_offset(0.0f, 0.0f) {

    m_platforms.resize(Constants::mapSize.y);
    for(auto i = 0u;i < Constants::mapSize.y; ++i) {
        m_platforms[i].resize(Constants::mapSize.x);
        for(auto j = 0u;j < Constants::mapSize.x; ++j) {
            m_platforms[i][j] = false;
        }
    }

    initVertices();
}


void GameMap::setPlatform(int x, int y) {
    m_platforms[y][x] = true;
}

void GameMap::removePlatform(int x, int y) {
    m_platforms[y][x] = false;
}

void GameMap::setOffset(const sf::Vector2f& offset) {
    m_offset = offset;
}

#include <iostream>
void GameMap::draw(Window& window) {
    for(auto y = 0u;y < Constants::mapSize.y;y++) {
        for(auto x = 0u;x < Constants::mapSize.x;x++) {
            if(!m_platforms[y][x]) {
                continue;
            }

            const sf::Vector2f totalOffset = m_offset + float(x) * Constants::mapSize.x * Constants::i - float(y) * Constants::mapSize.y * Constants::j;
            translateVertices(totalOffset);

            window.getWindow()->draw(m_vertices, 6, sf::PrimitiveType::Triangles);
            if(y == Constants::mapSize.y - 1 || (y < Constants::mapSize.y - 1 && !m_platforms[y + 1][x])) {
                window.getWindow()->draw(m_vertices + 6, 6, sf::PrimitiveType::Triangles);
            }

            if(x == Constants::mapSize.x - 1 || (x < Constants::mapSize.x - 1 && !m_platforms[y][x + 1])) {
                window.getWindow()->draw(m_vertices + 12, 6, sf::PrimitiveType::Triangles);
            }

            translateVertices(-totalOffset);

        }
    }

}

void GameMap::translateVertices(const sf::Vector2f& offset) {
    for(int i = 0;i < 18; ++i) {
        m_vertices[i].position += offset;
    }
}


void GameMap::initVertices() {
    sf::Color topColor = (m_color == WHITE) ? sf::Color(0, 0, 0, 255)      : sf::Color(255, 255, 255, 255);
    sf::Color frontColor = (m_color == WHITE) ? sf::Color(32, 32, 32, 255) : sf::Color(223, 223, 233, 255);
    sf::Color rightColor = (m_color == WHITE) ? sf::Color(48, 48, 48, 255) : sf::Color(207, 207, 207, 255);

    // Top
    m_vertices[0].position = Constants::i * (0.0f - Constants::platformSize.x * 0.5f) + Constants::j * (0.0f - Constants::platformSize.y * 0.5f);
    m_vertices[1].position = Constants::i * (0.0f - Constants::platformSize.x * 0.5f) + Constants::j * (0.0f + Constants::platformSize.y * 0.5f);
    m_vertices[2].position = Constants::i * (0.0f + Constants::platformSize.x * 0.5f) + Constants::j * (0.0f + Constants::platformSize.y * 0.5f);

    m_vertices[3].position = Constants::i * (0.0f + Constants::platformSize.x * 0.5f) + Constants::j * (0.0f + Constants::platformSize.y * 0.5f);
    m_vertices[4].position = Constants::i * (0.0f + Constants::platformSize.x * 0.5f) + Constants::j * (0.0f - Constants::platformSize.y * 0.5f);
    m_vertices[5].position = Constants::i * (0.0f - Constants::platformSize.x * 0.5f) + Constants::j * (0.0f - Constants::platformSize.y * 0.5f);

    // Front
    m_vertices[6].position = m_vertices[5].position;
    m_vertices[7].position = m_vertices[4].position;
    m_vertices[8].position = m_vertices[4].position + Constants::k * 100.0f;

    m_vertices[9].position  = m_vertices[8].position;
    m_vertices[10].position = m_vertices[0].position + Constants::k * 100.0f;
    m_vertices[11].position = m_vertices[0].position;

    // Right
    m_vertices[12].position = m_vertices[4].position;
    m_vertices[13].position = m_vertices[3].position;
    m_vertices[14].position = m_vertices[3].position + Constants::k * 100.0f;

    m_vertices[15].position = m_vertices[14].position;
    m_vertices[16].position = m_vertices[12].position + Constants::k * 100.0f;
    m_vertices[17].position = m_vertices[12].position;

    for(int i = 0;i < 18; ++i) {
        if(i < 6) {
            m_vertices[i].color = topColor;
        } else if(i < 12) {
            m_vertices[i].color = frontColor;
        } else {
            m_vertices[i].color = rightColor;
        }
    }
}