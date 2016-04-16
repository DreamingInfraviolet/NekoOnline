#pragma once
#include "matrixd.h"
#include <vector>
#include <chrono>
#include <Windows.h>

class Pet;

class AnimationComponent
{
private:

   //The list of possible states
    enum Frame
    {
        FRAME_SITTING, FRAME_SLEEP0_LICK_PAW, FRAME_SLEEP1_SCRATCH_LEG_0, FRAME_SLEEP2_SCRATCH_LEG_1,
        FRAME_SLEEP3_YAWN, FRAME_SLEEP3_Z_0, FRAME_SLEEP4_Z_1, FRAME_STARTLED, FRAME_RUN_UP_0, FRAME_RUN_UP_1,
        FRAME_RUN_DOWN_0, FRAME_RUN_DOWN_1, FRAME_RUN_LEFT_0, FRAME_RUN_LEFT_1, FRAME_RUN_RIGHT_0, FRAME_RUN_RIGHT_1,
        FRAME_RUN_LEFT_UP_0, FRAME_RUN_LEFT_UP_1, FRAME_RUN_RIGHT_UP_0, FRAME_RUN_RIGHT_UP_1, FRAME_RUN_LEFT_DOWN_0,
        FRAME_RUN_LEFT_DOWN_1, FRAME_RUN_RIGHT_DOWN_0, FRAME_RUN_RIGHT_DOWN_1, FRAME_SCRATCH_UP_0, FRAME_SCRATCH_UP_1,
        FRAME_SCRATCH_DOWN_0, FRAME_SCRATCH_DOWN_1, FRAME_SCRATCH_LEFT_0, FRAME_SCRATCH_LEFT_1, FRAME_SCRATCH_RIGHT_0,
        FRAME_SCRATCH_RIGHT_1, FRAME_COUNT
    };

        //A mapping from Frame to its coordinates in the sprite image
    math::vec2i frameToCoord[FRAME_COUNT] = {{0,0}, {1,0}, {2,0}, {3,0}, {0,1}, {1,1}, {2,1}, {3,1},
        {0,2}, {1,2}, {2,2}, {3,2}, {0,3}, {1,3}, {2,3}, {3,3}, {0,4}, {1,4}, {2,4},
        {3,4}, {0,5}, {1,5}, {2,5}, {3,5}, {0,6}, {1,6}, {2,6}, {3,6}, {0,7}, {1,7},
        {2,7}, {3,7}};

public:
    enum Sequence
    {
        SEQUENCE_IDLE, SEQUENCE_FALLING_ASLEEP, SEQUENCE_SLEEPING, SEQUENCE_WAKING_SLOWLY, SEQUENCE_WAKING_STARTLED, SEQUENCE_RUNNING_LEFT,
        SEQUENCE_RUNNING_RIGHT, SEQUENCE_RUNNING_UP, SEQUENCE_RUNNING_DOWN, SEQUENCE_RUNNING_LEFT_TOP, SEQUENCE_RUNNING_RIGHT_TOP,
        SEQUENCE_RUNNING_LEFT_BOTTOM, SEQUENCE_RUNNING_RIGHT_BOTTOM, SEQUENCE_SCRATCHING_LEFT, SEQUENCE_SCRATCHING_RIGHT,
        SEQUENCE_SCRATCHING_TOP, SEQUENCE_SCRATCHING_BOTTOM, SEQUENCE_COUNT
    };

private:
    std::vector<Frame> sequences[SEQUENCE_COUNT]
    {
        { FRAME_SITTING },      //IDLE
        { FRAME_SITTING, FRAME_SLEEP0_LICK_PAW, FRAME_SITTING, FRAME_SLEEP0_LICK_PAW, FRAME_SITTING, FRAME_SITTING, FRAME_SLEEP1_SCRATCH_LEG_0,
            FRAME_SLEEP2_SCRATCH_LEG_1, FRAME_SLEEP1_SCRATCH_LEG_0, FRAME_SLEEP2_SCRATCH_LEG_1, FRAME_SLEEP3_YAWN,
            FRAME_SLEEP3_YAWN},      //FALLING_ASLEEP
        {FRAME_SLEEP3_Z_0, FRAME_SLEEP4_Z_1},      //SLEEPING
        {FRAME_SLEEP3_YAWN, FRAME_SLEEP3_YAWN, FRAME_SLEEP0_LICK_PAW, FRAME_SITTING, FRAME_SLEEP0_LICK_PAW, FRAME_SITTING },      //WAKING_SLOWLY
        {FRAME_STARTLED},      //WAKING_STARTLED
        {FRAME_RUN_LEFT_0, FRAME_RUN_LEFT_1},      //RUNNING_LEFT,
        {FRAME_RUN_RIGHT_0, FRAME_RUN_RIGHT_1},      //RUNNING_RIGHT,
        {FRAME_RUN_UP_0, FRAME_RUN_UP_1},      //RUNNING_UP,
        {FRAME_RUN_DOWN_0,FRAME_RUN_DOWN_1},      //RUNNING_DOWN,
        {FRAME_RUN_LEFT_UP_0,FRAME_RUN_LEFT_UP_1},      //RUNNING_LEFT_TOP,
        {FRAME_RUN_RIGHT_UP_0, FRAME_RUN_RIGHT_UP_1},      //RUNNING_RIGHT_TOP,
        {FRAME_RUN_LEFT_DOWN_0, FRAME_RUN_LEFT_DOWN_1},      //RUNNING_LEFT_BOTTOM,
		{FRAME_RUN_RIGHT_DOWN_0,FRAME_RUN_RIGHT_DOWN_1},      //RUNNING_RIGHT_BOTTOM,
        {FRAME_SCRATCH_LEFT_0,FRAME_SCRATCH_LEFT_1},      //SCRATCHING_LEFT,
        {FRAME_SCRATCH_RIGHT_0,FRAME_SCRATCH_RIGHT_1},      //SCRATCHING_RIGHT,
        {FRAME_SCRATCH_UP_0,FRAME_SCRATCH_UP_1},      //SCRATCHING_TOP,
        {FRAME_SCRATCH_DOWN_0,FRAME_SCRATCH_DOWN_1}      //SCRATCHING_BOTTOM
	};


	const math::vec2i gridSize = math::vec2i(4, 8);

	//This determines how often a frame should change.
	const int animationChangeInterval = 3;

	//This variable is used to determine whether the animation frame should be changed
	int animationChangeCounter = 0;

	Sequence currentSequence = SEQUENCE_IDLE;
	int framePosition = 0;
public:


    AnimationComponent();
    ~AnimationComponent();

	/** Sets a sequence, resetting to the beginning */
	void setCurrentSequence(Sequence seq);

	/** Draws the pet and advanced the animation frame by one :3 */
	void draw(HWND hwnd, Pet* pet);

	Sequence getAnimationFromDirection(math::vec2 delta);
};