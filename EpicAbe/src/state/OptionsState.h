/*
 * This class controls
 *		- Creating an image for the options screen.
 *		- Logic for button input.
 *		- Changing state based on button input.
 *		- Adjusting volume based on button input.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GameState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
class StateMachine;

namespace gef
{
	Texture;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class OptionsState : public GameState
{
public:
	OptionsState(StateMachine* sm);
	~OptionsState() override;

	void onEnter() override;
	void onExit() override;
	void handleInput(float dt) override;
	bool update(float dt) override;
	void render() override;

protected:

private:
	StateMachine* stateMachine;

	gef::Texture* initStateGraphic(gef::Platform& platform_);

	float stateTimer;
	float totalTimeElapsed;
	bool  moveToMainMenu;
};

/////////////////////////////////////////////////////////////////////////////////////////////////