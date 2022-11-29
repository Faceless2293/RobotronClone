#define COINCOUNT 50
#define ENEMYCOUNT 5
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
	struct Scientist 
	{
		Vector2* position;
		Rect* sourceRect;
		Texture2D* texture;
		int spriteTraversal;
		int frame;
		int currentFrameTime;
		unsigned char direction;
		bool isMoving = false;
		bool isShooting = false;
		bool dead;
	};
	


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
	struct MovingEnemy 
	{
		Vector2* position;
		Texture2D* texture;
		Rect* rect;
		float speed;
		int direction;
	};
	

	//Civillian data
	Texture2D* _civillianTexture;
	Vector2* _civillianPosition;
	Rect* _civillianRect;
	float _civillianSpeed;
	int _civillianFrameCount;
	int _civillianDirection;

	//Civillian Target
	Vector2* _targetPosition;
	Rect* _targetRect;

	// Position for String
	Vector2* _stringPosition;

	//structs 
	Scientist* _player;
	MovingEnemy* _enemy[ENEMYCOUNT];

	//Input method
	void AttackInput(int elapsedTime);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckEnemyCollision();
	


	//Update methods
	void UpdatePlayer(int elapsedTime);
	void UpdateCoin(int elapsedTime);
	void PlayerMovement(int elapsedTime);
	void SpawnBullet();
	void EnemyMovement(MovingEnemy* _enemy, int elapsedTime);
	void CivillianMovement(int elapsedTime);

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