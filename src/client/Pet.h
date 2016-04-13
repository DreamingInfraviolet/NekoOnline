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

	void update()
	{
		switch (state)
		{
		case State::ASLEEP:
			break;
		case State::HAPPY:
			break;
		case State::NEUTRAL:
			if (currentTarget == math::vec2i(-1))
			{
				currentTarget = getRandomTarget();
			}
			else if (currentTarget.distance(pos) < 5)
			{
				currentTarget = math::vec2i(-1);
			}
			else
			{
				math::vec2i delta = currentTarget - pos;
				if (delta.x != 0)
					delta.x = delta.x / delta.x*(delta.x > 0 ? 1 : -1);
				if (delta.y != 0)
					delta.y = delta.y / delta.y*(delta.y > 0 ? 1 : -1);
				pos += delta;
			}
			break;
		case State::CURIOUS:
			break;
		}
	}

	math::vec2i getRandomTarget()
	{
		return math::vec2i(rand(1, 500), rand(1, 500));
	}



};

