#define COINCOUNT 50
#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Player : public Game
{
private:
	// Data to represent Player
	Vector2* _playerPosition;
	Rect* _playerSourceRect;
	Texture2D* _playerTexture;
	int _playerSpriteTraversal;
	int _playerFrame;
	int _playerCurrentFrameTime;
	unsigned char _playerDirection;
	bool _isMoving = false;
	bool _isShooting = false;


	//constant value
	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;

	//Pause Menu Variables
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;

	//Start Menu Variables
	Texture2D* _startBackground;
	Rect* _startRectangle;
	Vector2* _startStringPosition;
	bool _started;

	// Data to represent Bullet
	int _frameCount;
	Rect* _bulletRect;
	Texture2D* _bulletTexture;
	Vector2* _bulletPosition;
	int _bulletDirection;
	const float _cBulletSpeed;

	//Data to represent Enemy
	Vector2* _enemyPosition;
	Texture2D* _enemyTexture;
	Rect* _enemyRect;
	float _enemySpeed;
	int _enemyFrameCount;

	//Civillian data
	Texture2D* _civillianTexture;
	Vector2* _civillianPosition;
	Rect* _civillianRect;
	float _civillianSpeed;
	int _civillianFrameCount;


	// Position for String
	Vector2* _stringPosition;

	//Input method
	void AttackInput(int elapsedTime);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();

	//Update methods
	void UpdatePlayer(int elapsedTime);
	void UpdateCoin(int elapsedTime);
	void SpawnBullet();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Player(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Player();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};