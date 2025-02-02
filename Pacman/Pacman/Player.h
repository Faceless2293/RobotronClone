#pragma once
#define BULLETCOUNT 50
#define ENEMYCOUNT 5
#define CIVILLIANCOUNT 5

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
	const int _cLoadingFrameTime;

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
	Vector2* _startPosition;
	bool _started;

	//Loading Screen
	Texture2D* _loadingTexture;
	Rect* _loadingRect;
	bool _isLoading;
	int _loadingCurrentFrameTime;
	int _loadingFrame;

	//Game Over Variables
	Texture2D* _gameOverBackground;
	Rect* _gameOverRect;
	

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
		bool isDead;
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
	MovingEnemy* _enemy[ENEMYCOUNT];
	Civillian* _civillian[CIVILLIANCOUNT];
	Bullet* _bullet[BULLETCOUNT];
	
	//Audio
	SoundEffect* _oof;
	SoundEffect* _powerUp;
	SoundEffect* _ohNo;
	SoundEffect* _background;

	//Input method
	bool AttackInput(int elapsedTime);
	void PlayerMovement(int elapsedTime);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckCivillianViewportCollision();
	bool CheckCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	bool CheckObjectPosition(float objectPositionX, float objectPositionY);
	void ChangeDirection(int enemyDirection);

	//Update methods
	void UpdatePlayer(int elapsedTime);
	void UpdateEnemy(int elapsedTime);
	void UpdateCivillian(Civillian* _civillian, int elapsedTime);
	void BulletMovement(Bullet*, int direction, int elapsedTime);
	void EnemyMovement(MovingEnemy* _enemy1,MovingEnemy* _enemy2, float civillianPositionX, float civillianPositionY, int elapsedTime);
	void CivillianMovement(Civillian* _civillian, int elapsedTime);
	void UpdateLoading(int elapsedTime);

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