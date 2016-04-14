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
			delta *= 2;
			pos += delta;
		}
		break;
	case State::CURIOUS:
		break;
	}
}