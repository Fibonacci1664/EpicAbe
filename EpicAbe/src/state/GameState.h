#pragma once
#include <system/platform.h>

//#include "../../Game.h"

// Forward decls
namespace gef
{
	class SpriteRenderer;
	class Font;
}

class StateMachine;

class GameState
{
public:

	GameState();
	virtual ~GameState();

	virtual void onEnter() = 0;
	virtual void onExit() = 0;
	virtual void handleInput(float dt) = 0;
	virtual bool update(float dt) = 0;
	virtual void render() = 0;

protected:

	StateMachine* stateMachine;
	
private:
	
};