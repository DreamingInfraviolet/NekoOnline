#include "animation.h"
#include "pet.h"
#include <cassert>

AnimationComponent::AnimationComponent()
{
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::setCurrentSequence(AnimationComponent::Sequence seq)
{
	if (currentSequence == seq)
		return;

	currentSequence = seq;
	framePosition = 0;
	animationChangeCounter = 0;
}

void AnimationComponent::draw(HWND hwnd, Pet* pet)
{
	math::vec2i index = frameToCoord[sequences[currentSequence][framePosition]];
	math::vec2i squareSize = pet->size / gridSize;
	math::vec2i start = index*squareSize;
	math::vec2i end = (index+1)*squareSize;

	pet->draw(hwnd, start.x, start.y, end.x, end.y);

	++animationChangeCounter;
	if (animationChangeCounter%animationChangeInterval == 0)
		framePosition = (framePosition + 1) % sequences[currentSequence].size();
}

AnimationComponent::Sequence AnimationComponent::getAnimationFromDirection(math::vec2i delta)
{
	bool left = delta.x < 0;
	bool right = delta.x > 0;
	bool up = delta.y < 0;
	bool down = delta.y > 0;

	if (!left && !right && !up && !down)
		return SEQUENCE_IDLE;
	if (left)
	{
		if (!up && !down)
			return SEQUENCE_RUNNING_LEFT;
		else if (up)
			return SEQUENCE_RUNNING_LEFT_TOP;
		else if (down)
			return SEQUENCE_RUNNING_LEFT_BOTTOM;
	}
	if (right)
	{
		if (!up && !down)
			return SEQUENCE_RUNNING_RIGHT;
		else if (up)
			return SEQUENCE_RUNNING_RIGHT_TOP;
		else if (down)
			return SEQUENCE_RUNNING_RIGHT_BOTTOM;
	}
	if (up)
		return SEQUENCE_RUNNING_UP;
	if (down)
		return SEQUENCE_RUNNING_DOWN;

	assert(0);
}