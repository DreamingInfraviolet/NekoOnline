#include "animation.h"
#include "pet.h"


AnimationComponent::AnimationComponent()
{
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::setCurrentSequence(AnimationComponent::Sequence seq)
{
	currentSequence = seq;
	framePosition = 0;
}

void AnimationComponent::draw(HWND hwnd, Pet* pet)
{
	math::vec2i index = frameToCoord[sequences[currentSequence][framePosition]];
	math::vec2i squareSize = pet->size / gridSize;
	math::vec2i start = index*squareSize;
	math::vec2i end = (index+1)*squareSize;

	pet->draw(hwnd, start.x, start.y, end.x, end.y);

	framePosition = (framePosition + 1) % sequences[currentSequence].size();
}