#pragma once
#include "Sprite.h"
#include "rand.h"

class Pet : public Sprite
{
private:
	enum class State { ASLEEP, HAPPY, CURIOUS, NEUTRAL, ACTIVE };

	State state = State::ACTIVE;

	int sleepyness = 0;
	int sleepThreshold = 10000;
	
	
	math::vec2 currentTarget = math::vec2(-1);
	math::vec2 originalDeltaToTarget;

public:
	Pet(const std::string& path);
	~Pet();

	math::vec2 centre()
	{
		return pos + math::vec2(size.x, size.y) / 2;
	}

	void update(math::vec2 screenSize);

	math::vec2 getRandomTarget(math::vec2 screenSize)
	{
		return math::vec2(rand(0.0f, screenSize.x), rand(0.0f, screenSize.y));
	}

	math::vec2 getTargetDelta()
	{
		return originalDeltaToTarget;
	}
};

