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

class LevelComplete : public GameState
{
public:
	LevelComplete(StateMachine* sm);
	~LevelComplete() override;

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