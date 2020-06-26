#ifndef INTROSTATE_H_INCLUDED
#define INTROSTATE_H_INCLUDED

#include "StateBase.h"

class IntroState: public BaseState {
public:

    IntroState(StateManager* manager);

    virtual ~IntroState() { }

    virtual void onCreate();
    virtual void onDestroy();

    virtual void onActivate();
    virtual void onDeactivate();

    virtual void draw();
    virtual void update(const sf::Time& dt);

private:
    float m_elapsedTime;

};

#endif // INTROSTATE_H_INCLUDED
