/*
 * This class controls
 *		- Creating an image for the splash screen.
 *		- Timing how long the splash screen displays for.
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
	class Texture;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class SplashState : public GameState
{
public:
	SplashState(StateMachine* sm);
	~SplashState() override;

	
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
};

/////////////////////////////////////////////////////////////////////////////////////////////////