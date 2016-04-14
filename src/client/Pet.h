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
	
	
	math::vec2i currentTarget = math::vec2i(-1);

public:
	Pet();
	~Pet();

	math::vec2i centre()
	{
		return pos + size / 2;
	}

	void update();

	math::vec2i getRandomTarget()
	{
		return math::vec2i(rand(1, 500), rand(1, 500));
	}



};

