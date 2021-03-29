#pragma once
#include "GameState.h"

class StateMachine;

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

