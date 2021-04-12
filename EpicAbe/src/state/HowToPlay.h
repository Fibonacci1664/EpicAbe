/*
 * This class controls
 *		- Creating an image for the how to play screen.
 *		- Logic for button input.
 *		- Changing state based on button input.
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
class Platform;

namespace gef
{
	class Texture;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class HowToPlay : public GameState
{
public:
	HowToPlay(StateMachine* sm);
	~HowToPlay() override;

	void onEnter() override;
	void onExit() override;
	void handleInput(float dt) override;
	bool update(float dt) override;
	void render() override;

protected:

private:

	StateMachine* stateMachine;
	gef::Texture* initStateGraphic(gef::Platform& platform_);
	bool back;
};

/////////////////////////////////////////////////////////////////////////////////////////////////