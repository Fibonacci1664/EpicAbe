#pragma once
#include <map>
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
class SplashState;
class MenuState;
class GameState;
class Level_1_Intro;
class Level_1_State;
class LevelComplete;
class PauseState;
class OptionsState;
class HowToPlay;
class b2World;
class Game;
class GameOver;

namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Renderer3D;
	class Font;
	class InputManager;
	class SonyController;
	class SonyControllerInputManager;
	class AudioManager;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class StateMachine
{
public:
	StateMachine(gef::Platform& platform_);
	~StateMachine();

	void initGameWorld(Game* theGame, b2World* world, gef::AudioManager* audioMan);
	void initRend(gef::SpriteRenderer* spRend, gef::Renderer3D* rend3D, gef::Font* font_);
	void initInputControl(gef::InputManager* im, const gef::SonyController* sc, gef::SonyControllerInputManager* scim);
	void initStates();
	void onEnter();
	void onExit();
	void handleInput(float dt);
	bool update(float dt);
	void render();
	
	void setState(std::string newState);
	GameState* getCurrentState();
	gef::Platform& getPlatform();
	gef::SpriteRenderer* getSpriteRenderer();
	gef::Font* getFont();
	gef::InputManager* getInputManagaer();
	const gef::SonyController* getSonyController();
	gef::SonyControllerInputManager* getSonyControllerIM();
	b2World* getPhysicsWorld();
	gef::Renderer3D* get3DRenderer();
	Game* getGame();
	gef::AudioManager* getAudioManager();

private:
	
	void initStateMap();

	SplashState* splashState;
	MenuState* mainMenuState;
	Level_1_Intro* level_1_intro_state;
	Level_1_State* level_1_state;
	LevelComplete* levelCompleteState;
	PauseState* pauseState;
	OptionsState* optionsState;
	HowToPlay* howToPlayState;
	GameOver* gameOverState;

	GameState* currentState;

	gef::Platform& platform;
	gef::SpriteRenderer* sprRend;
	gef::Renderer3D* renderer3D;
	gef::Font* font;

	gef::InputManager* input_manager_;
	const gef::SonyController* sc_;
	gef::SonyControllerInputManager* scim_;

	b2World* theWorld;
	Game* game;

	gef::AudioManager* audioManager;

	std::map<std::string, GameState*> states;
};

/////////////////////////////////////////////////////////////////////////////////////////////////