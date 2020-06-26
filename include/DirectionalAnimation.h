#ifndef DIRECTIONALANIMATION_H_INCLUDED
#define DIRECTIONALANIMATION_H_INCLUDED

#include "Animation.h"
#include "Common.h"

class DirectionalAnimation: public Animation {

    // ROW:             LEFT-TO-RIGHT   COLUMN:             TOP-BOTTOM
    // REVERSED-ROW:    RIGHT-TO-LEFT   REVERSED-COLUMN:    BOTTOM-TOP

    enum Direction { ROW, COLUMN };
public:

    virtual ~DirectionalAnimation() { }

    virtual Frame getCurrentFrame() const;
    virtual bool nextFrame();
    virtual bool previousFrame();
    virtual void reset();
    virtual bool isFinished() const;

protected:

    virtual bool initWithJson(nlohmann::json& parser);
    DirectionalAnimation* create() const;

private:
    void calculateDirectionVector();

    Direction       m_direction;
    sf::Vector2i    m_directionVector;

    bool            m_reversed;

    sf::Vector2i    m_startFramePosition;
    sf::Vector2i    m_endFramePosition;
    sf::Vector2i    m_currentFramePosition;

    sf::Vector2u    m_frameSize;
};

#endif // DIRECTIONALANIMATION_H_INCLUDED
