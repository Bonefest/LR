#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <vector>
#include "Game/Components.h"
#include "Window.h"

class GameMap {
public:

    GameMap(PlayerColor color);

    void setPlatform(int i, int j);
    void removePlatform(int i, int j);

    void draw(Window& window);

    void setOffset(const sf::Vector2f& offset);
    sf::Vector2i convertToMapCoords(const sf::Vector2f& position) const;
    bool isSet(const sf::Vector2f& position) const;

private:
    void initVertices();
    void translateVertices(const sf::Vector2f& offset);

    std::vector<std::vector<bool>>  m_platforms;
    PlayerColor                     m_color;

    sf::Vertex                      m_vertices[18];
    sf::RectangleShape              m_background;
    //TOP-LEFT
    sf::Vector2f                    m_offset;

};

struct GameMaps {

    GameMaps(GameMap* l_blackMap, GameMap* l_whiteMap): blackMap(l_blackMap), whiteMap(l_whiteMap) { }

    GameMap* blackMap;
    GameMap* whiteMap;
};

#endif // GAMEMAP_H_INCLUDED
