#pragma once
#include "Sprite.h"
#include "rand.h"

class Pet : public Sprite
{
private:
	enum class State { ASLEEP, HAPPY, CURIOUS, NEUTRAL };

	State state = State::NEUTRAL;

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

	void update();

	math::vec2 getRandomTarget()
	{
		return math::vec2(rand(1, 500), rand(1, 500));
	}

	math::vec2 getTargetDelta()
	{
		return originalDeltaToTarget;
	}
};

