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

class PauseState : public GameState
{
public:
	PauseState(StateMachine* sm);
	~PauseState() override;

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
	bool unpause;
	bool gotoMainMenu;
	bool gotoOptions;
};

/////////////////////////////////////////////////////////////////////////////////////////////////