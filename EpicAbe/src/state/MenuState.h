#pragma once
#include "GameState.h"

namespace gef
{
	class Texture;
}

class Platform;

class MenuState : public GameState
{
public:
	MenuState(StateMachine* sm);
	~MenuState() override;

	void onEnter() override;
	void onExit() override;
	void handleInput(float dt) override;
	bool update(float dt) override;
	void render() override;

protected:

private:
	StateMachine* stateMachine;

	gef::Texture* initMenuGraphic(gef::Platform& platform_);
	gef::Texture* mainMenuGraphic;

	bool isStartGame;
	bool isOptions;
};

