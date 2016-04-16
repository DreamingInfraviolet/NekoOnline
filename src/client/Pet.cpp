#include "Pet.h"



Pet::Pet(const std::string& path)
	: Sprite(path)
{
}


Pet::~Pet()
{
}

void Pet::update()
{
	switch (state)
	{
	case State::ASLEEP:
		break;
	case State::HAPPY:
		break;
	case State::NEUTRAL:
		if (currentTarget.distance(pos) < 5)
		{
			currentTarget = getRandomTarget();
			originalDeltaToTarget = currentTarget - pos;
			
		}
		{
			auto currentDelta = (currentTarget - pos).normalize()*5;
			pos += currentDelta;
		}
		break;
	case State::CURIOUS:
		break;
	}
}

