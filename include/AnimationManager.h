#ifndef ANIMATIONMANAGER_H_INCLUDED
#define ANIMATIONMANAGER_H_INCLUDED

#include "ResourcesManager.h"
#include "Animation.h"
#include "Common.h"

#include "json.hpp"


#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

using Animations = map<string, Animation*>;
using Frames = map<string, Frame>;

/*
 * AnimationManager is responsible for managing and storing animations and discrete frames.
 * AnimationManager stores prototypes of animations and whenever new request is made -
 * new copy of animation returned.
 *
 * Animations are just the sequenced arrays of frames which stores how fast these frames
 * are moving (frame speed), is it looped, reversed and so on.
 *
 * Frames will be allocated only with PerFrameAnimation type because DirectionalAnimation
 * stores beginning and the end of animation rather than array of frames.
 *
 * Current AnimationManager version is allocating textures through texture manager at
 * the first time when it loads animations or frames. It's possible to control textures
 * counter and decrease/increase it whenever animation is deleted or allocated but
 * cause i'm not going to use too much textures this would be an overkill.
 *
 *
 */

class AnimationManager {
public:

    AnimationManager(TextureManager* textureManager);
    ~AnimationManager();

    bool loadAnimationsFromFile(const string& fileName, bool forceLoad = false);
    bool loadFramesFromFile(const string& fileName);

    AnimationPtr getAnimation(const string& animationName) const;
    bool hasAnimation(const string& animationName) const;

    Frame getFrame(const string& frameName);
    bool hasFrame(const string& frameName);

private:

    bool loadAnimation(const string& animationName, nlohmann::json& parser);
    void loadFrame(const string& frameName, nlohmann::json& parser);
    vector<Frame> loadFrames(const vector<string>& frameNames);


    void freeAnimations();

    TextureManager* m_texManager;
    Animations      m_animations;
    Frames          m_frames;
};


#endif // ANIMATIONMANAGER_H_INCLUDED
