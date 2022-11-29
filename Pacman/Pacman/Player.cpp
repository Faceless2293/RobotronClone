#include "Player.h"

#include <sstream>

Player::Player(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.2f), _cPlayerFrameTime(250), _cBulletSpeed(100)
{
	
	//Initialise enemy attributes
	_enemy[0] = new MovingEnemy();
	_enemy[0]->direction = 0;
	_enemy[0]->speed = 0.1f;

	// Initialise player attributes
	_player = new Scientist();
	_paused = false;
	_started = true;
	_player->currentFrameTime = 0;
	_player->frame = 0;
	_player->spriteTraversal = 0;
	_player->direction = 0;
	_player->dead = false;

	//Initialise Civillian attributes
	_civillian = new Civillian();
	_civillian->direction = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Player::~Player()
{
	delete _player->texture;
	delete _player->sourceRect;
	delete _player;
	delete _bulletTexture;
	delete _bulletRect;
	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;
	delete _startBackground;
	delete _startRectangle;
	delete _startStringPosition;
	delete _enemy[0]->texture;
	delete _enemy[0]->rect;
	delete _enemy[0]->position;
	delete _civillian;
	delete _civillian->texture;
	delete _civillian->rect;
	delete _civillian->position;
	delete _targetPosition;
	delete _targetRect;
	delete _gameOverBackground;
	delete _gameOverRect;
	delete _gameOverStringPosition;
}

void Player::LoadContent()
{
	// Load Pacman
	_player->texture = new Texture2D();
	_player->texture->Load("Textures/Player.png", false);
	_player->position = new Vector2(350.0f, 350.0f);
	_player->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Bullet
	_bulletTexture = new Texture2D();
	_bulletTexture->Load("Textures/PlayerBullet.png", false);
	_bulletRect = new Rect(0.0f, 0.0f, 32, 32);

	//Load Civillian
	_civillian->texture = new Texture2D();
	_civillian->texture->Load("Textures/Civillian.png", false);
	_civillian->position = new Vector2(200.0f, 200.0f);
	_civillian->rect = new Rect(0.0f, 0.0f, 32, 32);

	//Load Civillian Target
	_targetPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_targetRect = new Rect(0.0f, 0.0f, 32, 32);

	//Load Enemy
	_enemy[0]->texture = new Texture2D();
	_enemy[0]->texture->Load("Textures/Enemy.png", false);
	_enemy[0]->position = new Vector2((rand()% Graphics::GetViewportWidth()), (rand()%Graphics::GetViewportHeight()));
	_enemy[0]->rect = new Rect(0.0f, 0.0f, 64, 64);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Set Start parameters
	_startBackground = new Texture2D();
	_startBackground->Load("Textures/Transparency.png", false);
	_startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	
	//Set Game Over parameters
	_gameOverBackground = new Texture2D();
	_gameOverBackground->Load("Textures/Transparency.png", false);
	_gameOverRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_gameOverStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Player::Update(int elapsedTime)
{

	if (!_started)
	{
		if (!_paused)
		{
			
			PlayerMovement(elapsedTime);
			CheckViewportCollision();
			UpdatePlayer(elapsedTime);	
			EnemyMovement(_enemy[0], elapsedTime);
			CivillianMovement(elapsedTime);
			CheckEnemyCollision();
			
		}
	}
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//Check for button press to start game
	if (keyboardState->IsKeyDown(Input::Keys::SPACE)) 
	{
		_started = false;
	}
	
	// Check for button press to pause game
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE)) 
	{
		_pKeyDown = false;
	}
	
}

void Player::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _player->position->X << " Y: " << _player->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_player->texture, _player->position, _player->sourceRect); // Draws Pacman

		//Draw Pause Menu
	if (_paused) 
	{
		std::stringstream menuStream;
		menuStream << "Paused!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

		//Draw Start Menu
	if (_started) 
	{
		std::stringstream startStream;
		startStream << "Press Space to Start!";

		SpriteBatch::Draw(_startBackground, _startRectangle, nullptr);
		SpriteBatch::DrawString(startStream.str().c_str(), _startStringPosition, Color::Green);
	}

	//Draw Game Over Screen
	if (_player->dead == true) 
	{
		std::stringstream gameOverStream;
		gameOverStream << "Game Over!";

		SpriteBatch::Draw(_gameOverBackground, _gameOverRect, nullptr);
		SpriteBatch::DrawString(gameOverStream.str().c_str(), _gameOverStringPosition, Color::Red);
	}
	
	//Draw Enemy
	for (int i = 0; i < ENEMYCOUNT; i++) 
	{
		SpriteBatch::Draw(_enemy[0]->texture, _enemy[0]->position, _enemy[0]->rect);
	}
	

	//Draw Bullet
	if (_player-> isShooting = true)
	{
		SpriteBatch::Draw(_bulletTexture, _player->position, _bulletRect);
	}

	//Draw Civillian
	SpriteBatch::Draw(_civillian->texture, _civillian->position, _civillian->rect);

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}
void Player::AttackInput(int elapsedTime) 
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//check for input
	if (keyboardState->IsKeyDown(Input::Keys::LEFT)) 
	{
		_player-> isShooting = true;
		_bulletPosition->X -= _cBulletSpeed * elapsedTime;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RIGHT)) 
	{
		_player-> isShooting = true;
		_bulletPosition->X += _cBulletSpeed * elapsedTime;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::UP))
	{
		_player-> isShooting = true;
		_bulletPosition->Y -= _cBulletSpeed * elapsedTime;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::DOWN))
	{
		_player-> isShooting = true;
		_bulletPosition->Y += _cBulletSpeed * elapsedTime;
	}
}
void Player::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey) 
{

}
void Player::CheckViewportCollision() 
{
	// collide with viewport walls
	if (_player->position->X + _player->sourceRect->Width >= Graphics::GetViewportWidth())
	{
		_player->position->X = Graphics::GetViewportWidth() - _player->sourceRect->Width;
	}
	if (_player->position->X + _player->sourceRect->Width <= 0)
	{
		_player->position->X = 33 - _player->sourceRect->Width;
	}
	if (_player->position->Y + _player->sourceRect->Height >= Graphics::GetViewportHeight())
	{
		_player->position->Y = Graphics::GetViewportHeight() - _player->sourceRect->Height;
	}
	if (_player->position->Y + _player->sourceRect->Height <= 0)
	{
		_player->position->Y = 32 - _player->sourceRect->Height;
	}
}
void Player::UpdatePlayer(int elapsedTime) 
{
	//Frame Check
	if (_player->currentFrameTime > _cPlayerFrameTime)
	{
		_player->frame++;
		if (_player->frame >= 4)
		{
			_player->frame = 0;
		}
		_player->currentFrameTime = 0;
	}

	//Traverses the sprite sheet when direcitons are input
	_player->sourceRect->Y = _player->sourceRect->Height * _player->spriteTraversal;
	_player->sourceRect->X = _player->sourceRect->Width * _player->frame;
	_frameCount++;
}
void Player::PlayerMovement(int elapsedTime) 
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		_player->position->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_player->spriteTraversal = 2;
		_player-> isMoving = true;
	}

	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_player->position->X -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_player->spriteTraversal = 3;
		_player-> isMoving = true;
	}

	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_player->position->Y -= _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis
		_player->spriteTraversal = 1;
		_player-> isMoving = true;
	}

	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_player->position->Y += _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis
		_player->spriteTraversal = 0;
		_player-> isMoving = true;
	}
}
void Player::SpawnBullet() 
{
	
}
void Player::EnemyMovement(MovingEnemy* _enemy, int elapsedTime) 
{
	_enemy->speed = 0.01f;
	float targetX = _player->position->X - _enemy->position->X;
	float targetY = _player->position->Y - _enemy->position->Y;
	_enemy->position->X += targetX * _enemy->speed;
	_enemy->position->Y += targetY * _enemy->speed;
	
}
void Player::CivillianMovement(int elapsedTime) 
{
	_civillian->speed = 0.01f;
	/*float civillianTargetX = _civillianPosition->X - _enemy[0]->position->X;
	float civillianTargetY = _civillianPosition->Y - _enemy[0]->position->Y;
	_civillianPosition->X += civillianTargetX * _civillianSpeed;
	_civillianPosition->Y += civillianTargetY * _civillianSpeed;
	if (_civillianDirection == 0)
	{
		_civillianPosition->X += _civillianSpeed * elapsedTime;
	}
	else if (_civillianDirection == 1)
	{
		_civillianPosition->X -= _civillianSpeed * elapsedTime;
	}
	if (_civillianPosition->X + _civillianRect->Width >= Graphics::GetViewportWidth() && _civillianDirection == 0) 
	{
		_civillianDirection = 1;
	}
	else if (_civillianPosition->X <= 0) 
	{
		_civillianDirection = 0;
	}*/

	float civillianTargetX = _civillian->position->X + _targetPosition->X;
	float civillianTargetY = _civillian->position->Y + _targetPosition->Y;
	_civillian->position->X += civillianTargetX * _civillian->speed;
	_civillian->position->Y += civillianTargetY * _civillian->speed;

	if (_civillian->position == _targetPosition) 
	{
		_targetPosition->X = (rand() % Graphics::GetViewportWidth());
		_targetPosition->Y = (rand() % Graphics::GetViewportHeight());
	}
	
}
void Player::CheckCivillianViewportCollision() 
{
	if (_civillian->position->X + _civillian->rect->Width >= Graphics::GetViewportWidth())
	{
		_civillian->position->X = Graphics::GetViewportWidth() - _civillian->rect->Width;
	}
	if (_civillian->position->X + _civillian->rect->Width <= 0)
	{
		_civillian->position->X = 33 - _civillian->rect->Width;
	}
	if (_civillian->position->Y + _civillian->rect->Height >= Graphics::GetViewportHeight())
	{
		_civillian->position->Y = Graphics::GetViewportHeight() - _civillian->rect->Height;
	}
	if (_civillian->position->Y + _civillian->rect->Height <= 0)
	{
		_civillian->position->Y = 32 - _civillian->rect->Height;
	}
}
void Player::CheckEnemyCollision() 
{
	//Local variables
	int i = 0;
	int bottom1 = _player->position->Y + _player->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _player->position->X;
	int left2 = 0;
	int right1 = _player->position->X + _player->sourceRect->Width;
	int right2 = 0;
	int top1 = _player->position->Y;
	int top2 = 0;

	for (i = 0; i < ENEMYCOUNT; i++) 
	{
		//Populate variables with enemy data
		bottom2 = _enemy[i]->position->Y + _enemy[i]->rect->Height;
		left2 = _enemy[i]->position->X;
		right2 = _enemy[i]->position->X + _enemy[i]->rect->Width;
		top2 = _enemy[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_player->dead = true;
			i = ENEMYCOUNT;
		}
		
	}


}
