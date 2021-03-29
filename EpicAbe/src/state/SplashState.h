#pragma once
#include "GameState.h"

// FORWARD DECS
class StateMachine;
class Texture;

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
	gef::Texture* initSplashLogo(gef::Platform& platform_);
	gef::Texture* splashLogo;

	float stateTimer;
};

