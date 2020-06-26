#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include "Common.h"

#include <string>
#include <memory>
#include "json.hpp"

class Animation {
public:
    virtual ~Animation() { }

	virtual Frame getCurrentFrame() const = 0;
	virtual bool nextFrame() = 0;
	virtual bool previousFrame() = 0;
	virtual void reset() = 0;
	virtual bool isFinished() const = 0;

    void update(const sf::Time& dt);

	const sf::Time& getFrameDuration() const;
	void setFrameDuration(const sf::Time& time);

	void setLoop(bool loop);
	bool isLoop() const;

    inline sf::Texture* getTexture() { return m_texture; }

protected:
    friend class AnimationManager;

    virtual bool initWithJson(nlohmann::json& parser) = 0;

    // Prototype pattern
    virtual Animation* create() const = 0;

	bool		    m_finished;
	bool		    m_loop;
	sf::Time	    m_elapsedTime;
	sf::Time	    m_frameTime;

	sf::Texture*    m_texture;
};

using AnimationPtr = std::shared_ptr<Animation>;


#endif // ANIMATION_H_INCLUDED
