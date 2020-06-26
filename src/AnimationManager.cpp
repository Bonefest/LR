#include "AnimationManager.h"

#include <iostream>
#include <fstream>
#include "json.hpp"

#include "DirectionalAnimation.h"

using std::ifstream;
using nlohmann::json;

static void _logAnimationError(const string& animationName, const string& msg) {
    std::cerr << "[Error] Cannot load animation '" << animationName << "'\n";
    std::cerr << msg << "\n";
}

AnimationManager::AnimationManager(TextureManager* textureManager): m_texManager(textureManager) { }

AnimationManager::~AnimationManager() {
    freeAnimations();
}

void AnimationManager::freeAnimations() {
    for(auto animation: m_animations) {
        delete animation.second;
    }
    m_animations.clear();
}

bool AnimationManager::loadAnimationsFromFile(const string& fileName, bool forceLoad) {
    ifstream file(fileName);
    if(file.is_open()) {
        json parser;
        file >> parser;

        auto animationsIt = parser.find("animations");
        if(animationsIt == parser.end()) {
            std::cerr << "[Error] Cannot load animations from '" << fileName << "'!\n";
            std::cerr << "        Cannot find 'animations' tree\n";
            return false;
        }

        json animations = parser["animations"];

        for(auto animIt = animations.begin(); animIt != animations.end(); animIt++) {
            if(!loadAnimation(animIt.key(), animIt.value()) && !forceLoad) {
                freeAnimations();
                return false;
            }
        }

    }

    return true;
}

bool AnimationManager::loadAnimation(const string& animationName, nlohmann::json& parser) {
    auto animIt = m_animations.find(animationName);
    if(animIt != m_animations.end()) {
        return false;
    }

    Animation* animation = nullptr;

    if(parser["type"] == "directional") {
        animation = new DirectionalAnimation();
    }
//    else if(parser["type"] == "per_frame") {
//        PerFrameAnimation* _animation = new PerFrameAnimation();
//        auto frames = loadFrames(parser["frames"]);
//        if(frames.empty()) {
//            delete _animation;
//            return;
//        }
//        _animation->setFrames(frames);
//
//        l_animation = _animation;
//    }

    if(animation == nullptr) {
        return false;
    }

    animation->m_texture = m_texManager->get(parser.value("texture", ""));
    if(animation->m_texture == nullptr) {
        delete animation;
        _logAnimationError(animationName, string("Reason: cannot load texture '") + parser.value("texture", "") + "'");
        return false;
    }

    if(!animation->initWithJson(parser)) {
        delete animation;
        std::cerr << "[Error] Cannot load animation '" << animationName << "'\n";
        _logAnimationError(animationName, "Reason: json file type is incomplete\n");
        return false;
    }

    animation->setFrameDuration(sf::seconds(parser["frame_duration"]));
    animation->setLoop(parser.value("loop", false));

    m_animations.emplace(animationName, animation);

    return true;
}


AnimationPtr AnimationManager::getAnimation(const string& animationName) const {
    auto animationIt = m_animations.find(animationName);
    if(animationIt == m_animations.end()) {
        return nullptr;
    }

    return AnimationPtr(animationIt->second->create());
}

bool AnimationManager::hasAnimation(const string& animationName) const {
    return (m_animations.find(animationName) != m_animations.end());
}


vector<Frame> AnimationManager::loadFrames(const vector<string>& frameNames) {
    vector<Frame> result;
    for(auto name: frameNames) {
        auto frameIt = m_frames.find(name);
        if(frameIt == m_frames.end()) {
            result.clear();
            break;
        }

        result.push_back(frameIt->second);
    }

    return result;
}

bool AnimationManager::loadFramesFromFile(const string& fileName) {
    ifstream file(fileName);
    if(file.is_open()) {
        json parser;
        file >> parser;

        auto framesIt = parser.find("frames");
        if(framesIt == parser.end()) {
            std::cerr << "[Error] Cannot load frames from '" << fileName << "'\n";
            return false;
        }

        json frames = framesIt.value();

        for(auto frameIt = frames.begin(); frameIt != frames.end(); frameIt++) {
            loadFrame(frameIt.key(), frameIt.value());
        }

        file.close();
    }

    return true;
}

void AnimationManager::loadFrame(const string& frameName, nlohmann::json& parser) {
    if(m_frames.find(frameName) != m_frames.end()) {
        return;
    }

    Frame frame;

    frame.left   = parser["frame"]["x"];
    frame.top    = parser["frame"]["y"];
    frame.width  = parser["frame"]["w"];
    frame.height = parser["frame"]["h"];

    m_frames.emplace(frameName, frame);
}

Frame AnimationManager::getFrame(const string& frameName) {
    auto frameIt = m_frames.find(frameName);
    if(frameIt == m_frames.end()) {
        return Frame {0, 0, 0, 0};
    }

    return frameIt->second;
}
