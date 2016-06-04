#include "Pet.h"

Pet::Pet(const std::string& path)
	: Sprite(path)
{
}

Pet::~Pet()
{
}

void Pet::update(math::vec2 screenSize)
{
	switch (state)
	{
	case State::ASLEEP:
		break;
	case State::HAPPY:
		break;
	case State::ACTIVE:
		if (currentTarget.distance(pos) < 5)
		{
			currentTarget = getRandomTarget(screenSize);
			originalDeltaToTarget = currentTarget - pos;
			
		}
		{
			auto currentDelta = (currentTarget - pos).normalize()*10;
			pos += currentDelta;
		}
		break;
	case State::CURIOUS:
		break;
	case State::NEUTRAL:
		break;
	}
}

