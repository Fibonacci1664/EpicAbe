#ifndef _GAME_APP_H
#define _GAME_APP_H

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <system/application.h>
#include <box2d/box2d.h>
#include <gef.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
namespace gef
{
	class Platform;
	class Font;
	class SpriteRenderer;
	class Renderer3D;
	class InputManager;
	class SonyController;
	class SonyControllerInputManager;
	class AudioManager;
	class VolumeInfo;
}

class b2World;
class StateMachine;

/////////////////////////////////////////////////////////////////////////////////////////////////

class Game : public gef::Application
{
public:
	Game(gef::Platform& platform);
	void Init();
	void handleInput(float dt);
	bool Update(float dt);
	void Render();
	void CleanUp();

	float getGameVolume();
	void setGameVolume(float newVol);

private:
	void initFont();
	void initInputManager();
	void initWorld();
	void initStateMachine();
	void initAudio();

	// REND STUFF
	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;

	// FONT STUFF
	gef::Font* font_;

	// INPUT MANAGER STUFF
	gef::InputManager* im;
	const gef::SonyController* controller;
	gef::SonyControllerInputManager* scim;

	// PHYSICS WORLD STUFF
	b2World* world_;
	int32 velocityIterations;
	int32 positionIterations;

	// STATE MACHINE STUFF
	StateMachine* stateMachine;

	// AUDIO STUFF
	gef::AudioManager* audioManager;
	//Int32 sampleSound;
	Int32 gameMusic;
	gef::VolumeInfo* gameMusicVolinfo;
	float musicVolume;

	float fps_;
	bool quitGame;
};

#endif // _SCENE_APP_H

/////////////////////////////////////////////////////////////////////////////////////////////////