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

class Level_1_Intro : public GameState
{
public:
	Level_1_Intro(StateMachine* sm);
	~Level_1_Intro() override;

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