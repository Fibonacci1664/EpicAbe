#pragma once
#include "GameState.h"

namespace gef
{
	class Texture;
}

class Platform;

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