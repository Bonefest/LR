#ifndef EVENTMANAGER_H_INCLUDED
#define EVENTMANAGER_H_INCLUDED

/*
 *
 *
 * EventManager responsible for mapping Bindings to appropriate function objects
 * and executing corresponding functors whenever all events are satisfied.
 *
 * Binding responsible for storing an array of Events, number of satisfied events,
 * and EventDetails which will be shared with functor through EventManager.
 *
 * EventDetails stores information that could be useful for function that's binded
 * to Binding (mouse position, pressed keys).
 *
 * Event responsible for storing type (KeyPressed, MouseMoved, KeyDown etc) and value
 * where it's required (KeyPressed and key value)
 */

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "StateType.h"

using std::map;
using std::string;
using std::vector;
using std::function;

enum class EventType {
    Closed = sf::Event::Closed,
    Resized = sf::Event::Resized,
    LostFocus = sf::Event::LostFocus,
    GainedFocus = sf::Event::GainedFocus,
    TextEntered = sf::Event::TextEntered,
    KeyPressed = sf::Event::KeyPressed,
    KeyReleased = sf::Event::KeyReleased,
    MouseWheelScrolled = sf::Event::MouseWheelScrolled,
    MouseButtonPressed = sf::Event::MouseButtonPressed,
    MouseButtonReleased = sf::Event::MouseButtonReleased,
    MouseMoved = sf::Event::MouseMoved,
    Keyboard = sf::Event::Count + 1,
    Mouse,
    //Joystick
};

struct Event {
    Event(EventType l_type, sf::Keyboard::Key l_key = sf::Keyboard::Unknown): type(l_type), key(l_key) { }
    Event(EventType l_type, sf::Mouse::Button l_button): type(l_type), button(l_button) { }

    EventType type;
    union {
        sf::Keyboard::Key key;
        sf::Mouse::Button button;
    };
};

struct EventDetails {

    explicit EventDetails(string l_bindingName): bindingName(l_bindingName) {
        clear();
    }

    void clear() {
        pressedKeys.clear();
        pressedButtons.clear();

        mousePosition = sf::Vector2i(0, 0);
        textEnteredCode = 0;
    }

    string                      bindingName;
    vector<sf::Keyboard::Key>   pressedKeys;
    vector<sf::Mouse::Button>   pressedButtons;
    sf::Vector2i                mousePosition;
    float                       mouseScrollDelta;
    sf::Vector2i                newSize;
    sf::Uint32                  textEnteredCode;
};

class Binding {
public:
    explicit Binding(const string& name): m_name(name), m_details(name), m_activatedEvents(0) { }

    void addEvent(Event event) {
        m_events.push_back(event);
    }

    const EventDetails* getDetails() const {
        return &m_details;
    }

private:
    string          m_name;
    EventDetails    m_details;
    sf::Uint32      m_activatedEvents;
    vector<Event>   m_events;

    friend class EventManager;
};

using Bindings = map<string, Binding>;
using Callback = function<void(EventDetails*)>;
using Callbacks = map<string, Callback>;
using StateCallbacks = map<StateType, Callbacks>;

class EventManager {
public:
    bool addBinding(Binding binding);
    void removeBinding(const string& name);

    template <typename T>
    bool addCallback(StateType state, const string& bindingName, void(T::*method)(EventDetails*), T* instance) {
        return addCallback(state, bindingName, std::bind(method, instance, std::placeholders::_1));
    }
    bool addCallback(StateType state, const string& bindingName, Callback callback);

    void removeCallback(StateType state, const string& bindingName);

    void handleEvent(sf::Event& event);
    void update();

    void setCurrentState(StateType type);
    inline StateType getCurrentState() const { return m_currentState; }

    bool loadBindingsFromFile(const string& fileName);

private:
    void clearBinding(Binding& binding);

    Bindings        m_bindings;
    StateCallbacks  m_callbacks;
    StateType       m_currentState;
};

#endif // EVENTMANAGER_H_INCLUDED
