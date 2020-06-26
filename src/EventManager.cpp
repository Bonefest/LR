#include "EventManager.h"
#include <fstream>
#include <sstream>

bool EventManager::addBinding(Binding binding) {
    if(m_bindings.find(binding.m_name) != m_bindings.end()) {
        return false;
    }

    return m_bindings.emplace(binding.m_name, binding).second;
}

void EventManager::removeBinding(const string& name) {
    m_bindings.erase(name);
}

bool EventManager::addCallback(StateType state, const string& bindingName, Callback callback) {

    auto stateIt = m_callbacks.emplace(state, Callbacks()).first;

    if(stateIt->second.find(bindingName) != stateIt->second.end()) {
        return false;
    }

    return stateIt->second.emplace(bindingName, callback).second;
}

void EventManager::removeCallback(StateType type, const string& bindingName) {
    auto stateIt = m_callbacks.find(type);
    if(stateIt != m_callbacks.end())
        stateIt->second.erase(bindingName);
}

void EventManager::handleEvent(sf::Event& event) {
    for(auto& bindingPair: m_bindings) {
        Binding& binding = bindingPair.second;
        EventType eventType = EventType(event.type);

        for(auto l_event: binding.m_events) {
            if(l_event.type != eventType) continue;

            if( (eventType == EventType::KeyPressed || eventType == EventType::KeyReleased)) {

                if(l_event.key == event.key.code) {
                    binding.m_details.pressedKeys.push_back(l_event.key);
                    binding.m_activatedEvents++;

                    break;
                }
                continue;
            }

            else if( (eventType == EventType::MouseButtonPressed || eventType == EventType::MouseButtonReleased) &&
                      l_event.button == event.mouseButton.button) {

                if(l_event.button == event.mouseButton.button) {
                    binding.m_details.pressedButtons.push_back(l_event.button);
                    binding.m_details.mousePosition.x = event.mouseButton.x;
                    binding.m_details.mousePosition.y = event.mouseButton.y;
                    binding.m_activatedEvents++;

                    break;
                }
                continue;
            }

            else if(eventType == EventType::MouseMoved) {
                binding.m_details.mousePosition.x = event.mouseMove.x;
                binding.m_details.mousePosition.y = event.mouseMove.y;
            }

            else if(eventType == EventType::Resized) {
                binding.m_details.newSize.x = event.size.width;
                binding.m_details.newSize.y = event.size.height;
            }

            else if(eventType == EventType::MouseWheelScrolled) {
                binding.m_details.mouseScrollDelta = event.mouseWheelScroll.delta;
                binding.m_details.mousePosition.x = event.mouseWheelScroll.x;
                binding.m_details.mousePosition.y = event.mouseWheelScroll.y;
            }

            else if(eventType == EventType::TextEntered) {
                binding.m_details.textEnteredCode = event.text.unicode;
            }

            binding.m_activatedEvents++;
            break;

        }

    }
}

void EventManager::update() {
    for(auto& bindingPair: m_bindings) {
        auto& binding = bindingPair.second;


        for(auto event: binding.m_events) {
            if(event.type == EventType::Keyboard && sf::Keyboard::isKeyPressed(event.key)) {
                binding.m_details.pressedKeys.push_back(event.key);
                binding.m_activatedEvents++;
            }

            else if(event.type == EventType::Mouse && sf::Mouse::isButtonPressed(event.button)) {
                binding.m_details.pressedButtons.push_back(event.button);
                binding.m_activatedEvents++;
            }
        }

        StateCallbacks::iterator stateIt[2] = {
            m_callbacks.find(m_currentState),   // Current state
            m_callbacks.find(StateType::SYSTEM) // Global state should be executed too
        };

        for(int i = 0; i < 2; ++i) {
            if(stateIt[i] != m_callbacks.end()) {
                auto stateCallbackIt = stateIt[i]->second.find(binding.m_name);
                if(stateCallbackIt != stateIt[i]->second.end()) {
                    if(binding.m_activatedEvents == binding.m_events.size()) {
                        stateCallbackIt->second(&binding.m_details);
                    }
                }
            }
        }

        clearBinding(binding);
    }
}

void EventManager::clearBinding(Binding& binding) {
    binding.m_activatedEvents = 0;
    binding.m_details.clear();
}

#include <iostream>

bool EventManager::loadBindingsFromFile(const string& fileName) {
    std::ifstream file(fileName);
    if(!file.is_open()) {
        return false;
    }

    string line, bindingName, event;
    std::stringstream parser;

    while(!file.eof()) {
        std::getline(file, line);
        parser << line;

        parser >> bindingName;

        if(!parser.eof()) {
            if(m_bindings.find(bindingName) != m_bindings.end()) continue;

            Binding binding(bindingName);

            while(!parser.eof()) {
                parser >> event;
                std::size_t delimpos = event.find(':');
                if(delimpos == std::string::npos) {
                    binding.addEvent(Event(EventType(std::stoi(event))));
                } else {
                    binding.addEvent(Event(EventType(std::stoi(event.substr(0, delimpos))),
                                           sf::Keyboard::Key(std::stoi(event.substr(delimpos + 1)))));
                }
            }

            addBinding(binding);
            parser.clear();
        }
    }

    return true;
}

void EventManager::setCurrentState(StateType state) {
    m_currentState = state;
}
