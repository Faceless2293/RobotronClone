#define BULLETCOUNT 50
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
		bool isShooting;
		bool dead;
	};

	//constant value
	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;
	const int _cEnemyFrameTime;
	const int _cCivillianFrameTime;

	//Score Values
	int initialScore;
	int currentScore;
	int scoreIncrease;
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

	//Game Over Variables
	Texture2D* _gameOverBackground;
	Rect* _gameOverRect;
	Vector2* _gameOverStringPosition;
	

	// Data to represent Bullet
	struct Bullet 
	{
		Rect* rect;
		Texture2D* texture;
		Vector2* position;
		int direction;
		float speed;
	};
	

	//Data to represent Enemy
	struct MovingEnemy 
	{
		Vector2* position;
		Texture2D* texture;
		Rect* rect;
		float speed;
		int direction;
		int frame;
		int currentFrameTime;
	};
	

	//Civillian data
	struct Civillian 
	{
		Texture2D* texture;
		Vector2* position;
		Rect* rect;
		float speed;
		int frame;
		int currentFrameTime;
		int direction;
	};
	

	//Civillian Target
	Vector2* _targetPosition;
	Rect* _targetRect;

	// Position for String
	Vector2* _stringPosition;

	//structs 
	Scientist* _player;
	MovingEnemy* _enemy;
	Civillian* _civillian;
	Bullet* _bullet[BULLETCOUNT];
	
	//Audio
	SoundEffect* _oof;
	SoundEffect* _powerUp;
	SoundEffect* _ohNo;

	//Input method
	void AttackInput(int elapsedTime);
	void PlayerMovement(int elapsedTime);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckCivillianViewportCollision();
	bool CheckCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

	//Update methods
	void UpdatePlayer(int elapsedTime);
	void UpdateEnemy(int elapsedTime);
	void UpdateCivillian(int elapsedTime);
	void BulletMovement(int direction, int elapsedTime);
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