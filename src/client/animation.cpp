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

AnimationComponent::Sequence AnimationComponent::getAnimationFromDirection(math::vec2 delta)
{
	delta.y *= -1;
	delta.normalize();
	float pi = acos(-1.0f);
	float pio8 = pi / 8;
	float angle = delta.angle();
	if (angle < 0)
		angle += pi*2;

	if (angle < pio8)
		return SEQUENCE_RUNNING_RIGHT;
	if (angle > pio8 && angle < pio8 * 3)
		return SEQUENCE_RUNNING_RIGHT_TOP;
	if (angle > pio8 * 3 && angle < pio8 * 5)
		return SEQUENCE_RUNNING_UP;
	if (angle > pio8 * 5 && angle < pio8 * 7)
		return SEQUENCE_RUNNING_LEFT_TOP;
	if (angle > pio8 * 7 && angle < pio8 * 9)
		return SEQUENCE_RUNNING_LEFT;
	if (angle > pio8 * 9 && angle < pio8 * 11)
		return SEQUENCE_RUNNING_LEFT_BOTTOM;
	if (angle > pio8 * 11 && angle < pio8 * 13)
		return SEQUENCE_RUNNING_DOWN;
	if (angle > pio8 * 13 && angle < pio8 * 15)
		return SEQUENCE_RUNNING_RIGHT_BOTTOM;
	return SEQUENCE_RUNNING_RIGHT;
	
}