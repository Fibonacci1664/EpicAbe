#pragma once
#include "GameState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
class StateMachine;

namespace gef
{
	class Texture;
	class VolumeInfo;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class GameOver : public GameState
{
public:
	GameOver(StateMachine* sm);
	~GameOver() override;

	void onEnter() override;
	void onExit() override;
	void handleInput(float dt) override;
	bool update(float dt) override;
	void render() override;

protected:

private:
	StateMachine* stateMachine;

	gef::Texture* initStateGraphic(gef::Platform& platform_);
	void initStateAudio();

	float stateTimer;

	// AUDIO STUFF
	gef::AudioManager* audioManager;
	Int32 gameOverMusic;
	gef::VolumeInfo* gameOverMusicVolinfo;
	float musicVolume;
};

/////////////////////////////////////////////////////////////////////////////////////////////////