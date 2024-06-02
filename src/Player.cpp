#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "resource/Sprites/PacMans.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	sound_death = data.GetSound(AudioResource::pacmanDeath);

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PacManAnimation::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PacManAnimation::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PacManAnimation::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)PacManAnimation::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PacManAnimation::IDLE_LEFT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PacManAnimation::freeze_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PacManAnimation::freeze_UP, { 0, 2*n, n, n });
	sprite->SetAnimationDelay((int)PacManAnimation::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PacManAnimation::IDLE_DOWN, { 0, 3 * n, n, n });

	sprite->SetAnimationDelay((int)PacManAnimation::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)PacManAnimation::WALKING_RIGHT, { (float)i * n,  0, n, n });
	};

	sprite->SetAnimationDelay((int)PacManAnimation::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)PacManAnimation::WALKING_LEFT, { (float)i * n, n, n, n });
	};

	sprite->SetAnimationDelay((int)PacManAnimation::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 3; ++i){
		sprite->AddKeyFrame((int)PacManAnimation::WALKING_UP, { (float)i * n, 2*n, n, n });
	};

	sprite->SetAnimationDelay((int)PacManAnimation::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 3; ++i){
		sprite->AddKeyFrame((int)PacManAnimation::WALKING_DOWN, { (float)i * n, 3 * n, n, n });
    };

	sprite->SetAnimationDelay((int)PacManAnimation::DYING, ANIM_DELAY);
	for (i = 0; i < 12; ++i){
		sprite->AddKeyFrame((int)PacManAnimation::DYING, { (float)i * n, 4*n, n, n });
    };

	//chae form dead to dying so one state has animation and the other is just tje dead character

	sprite->SetAnimationDelay((int)PacManAnimation::CLOSED, ANIM_DELAY);
	sprite->AddKeyFrame((int)PacManAnimation::CLOSED, { 0, 4 * n, n, n });

	sprite->SetAnimationDelay((int)PacManAnimation::HIDDEN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PacManAnimation::HIDDEN, { 4*n, 0, n, n });

	sprite->SetAnimation((int)PacManAnimation::HIDDEN);

	return AppStatus::OK;
}
bool Player::IntroUpdate(bool turn) 
{
	bool end = false;
	if (turn) {
		position.x += PacMan_SPEED;
		if (state == State::freeze) WalkRight();
		else {
			if (!IsLookingRight()) ChangeAnimRight();
		}
	}
	else {
		position.x += -PacMan_SPEED;
		if (state == State::freeze) WalkLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}
	}

	if (position.x == WINDOW_WIDTH) end = true;
	return end;
}
int Player::GetScore()
{
	return score;
}
int Player::AddLives()
{
	return lives;
}
void Player::StartScore()
{
	score = 0;
}
void Player::IncreaseScore(int n)
{
	score += n;
}

Point Player::GetDirection() 
{
	return dir;
}


void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Player::Win() 
{
	SetAnimation((int)PacManAnimation::CLOSED);
}

void Player::Intro(int count) {
	if(count <= 60) SetAnimation((int)PacManAnimation::CLOSED);
	else SetAnimation((int)PacManAnimation::HIDDEN);
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PacManAnimation Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PacManAnimation)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::freeze;
	if (IsLookingRight())	SetAnimation((int)PacManAnimation::IDLE_RIGHT);
	else if (IsLookingUp())  SetAnimation((int)PacManAnimation::freeze_UP);
	else if (IsLookingDown())  SetAnimation((int)PacManAnimation::IDLE_DOWN);
	else					SetAnimation((int)PacManAnimation::IDLE_LEFT);
}
Point Player::GetPosition() {
	return position;
}
void Player::LoseLives()
{
	--lives;
}
void Player::setLives(int l)
{
	lives = l;
}
void Player::Defeat() {
	lose = true;
	if (count == 0) {
		PlaySound(sound_death);
		SetAnimation((int)PacManAnimation::DYING);
		LoseLives();
	}

	count++;

	if (count < 150) {
		if (count < 48) {
			Sprite* sprite = dynamic_cast<Sprite*>(render);
			sprite->Update();
		}
		else SetAnimation((int)PacManAnimation::HIDDEN);
	}
	else {
		lose = false;
		count = 0;
		SetAnimation((int)PacManAnimation::CLOSED);
	}
}
void Player::ChangeAnimUp()
{
	look = Look::UP;
	switch (state)
	{
	case State::freeze:	 SetAnimation((int)PacManAnimation::freeze_UP);    break;
	case State::moving: SetAnimation((int)PacManAnimation::WALKING_UP); break;
	}
}
void Player::WalkLeft()
{
	state = State::moving;
	look = Look::LEFT;
	SetAnimation((int)PacManAnimation::WALKING_LEFT);
}
void Player::WalkRight()
{
	state = State::moving;
	look = Look::RIGHT;
	SetAnimation((int)PacManAnimation::WALKING_RIGHT);
}

void Player::WalkDown()
{
	state = State::moving;
	look = Look::DOWN;
	SetAnimation((int)PacManAnimation::WALKING_DOWN);
}
void Player::StartDying()
{
	state = State::DYING;
	
	SetAnimation((int)PacManAnimation::DYING);
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::freeze:	 SetAnimation((int)PacManAnimation::IDLE_RIGHT);    break;
	case State::moving: SetAnimation((int)PacManAnimation::WALKING_RIGHT); break;
	}
}
void Player::WalkUp()
{
	state = State::moving;
	look = Look::UP;
	SetAnimation((int)PacManAnimation::WALKING_UP);
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::freeze:	 SetAnimation((int)PacManAnimation::IDLE_LEFT);    break;
	case State::moving: SetAnimation((int)PacManAnimation::WALKING_LEFT); break;
	}
}

void Player::ChangeAnimDown()
{
	look = Look::DOWN;
	switch (state)
	{
	case State::freeze:	 SetAnimation((int)PacManAnimation::IDLE_DOWN);    break;
	case State::moving: SetAnimation((int)PacManAnimation::WALKING_DOWN); break;
	}
}
void Player::Update()
{
	//all movement in move
	Move();   

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::Move()
{
	AABB box;
	int prev_x = position.x;
	int prev_y = position.y;

	//checks which way the player wants to turn next
	if (IsKeyPressed(KEY_UP) or IsKeyDown(KEY_UP))
	{
		turn = Look::UP;
		state = State::moving;
	}
	else if (IsKeyPressed(KEY_DOWN) or IsKeyDown(KEY_DOWN)) {
		turn = Look::DOWN;
		state = State::moving;
	}
	else if (IsKeyPressed(KEY_RIGHT) or IsKeyDown(KEY_RIGHT)) {
		turn = Look::RIGHT;
		state = State::moving;
	}
	else if (IsKeyPressed(KEY_LEFT) or IsKeyDown(KEY_LEFT)) {
		turn = Look::LEFT;
		state = State::moving;
	}

	//checks if the turn is possible
	if (turn != look) {
		switch (turn) {
		case Look::UP:
			position.y -= PacMan_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) ChangeAnimUp();
			position.y = prev_y;
			break;
		case Look::DOWN:
			position.y += PacMan_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) ChangeAnimDown();
			position.y = prev_y;
			break;
		case Look::LEFT:
			position.x -= PacMan_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box)) ChangeAnimLeft();
			position.x = prev_x;
			break;
		case Look::RIGHT:
			position.x += PacMan_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) ChangeAnimRight();
			position.x = prev_x;
			break;
		}
	}

	if (look == Look::LEFT)
	{
		position.x += -PacMan_SPEED;
		if (state == State::freeze)WalkLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			position.x = prev_x;
			if (state == State::moving) Stop();
		}
		if (position.x == 0) {
			position.x = WINDOW_WIDTH;
			ChangeAnimRight();
		}
		
	}
	else if (look == Look::RIGHT)
	{
		position.x += PacMan_SPEED;
		if (state == State::freeze) WalkRight();
		else
		{
			if (!IsLookingRight()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			position.x = prev_x;
			if (state == State::moving) Stop();
		}
		if (position.x == WINDOW_WIDTH - 8) {
			position.x = 0;
			ChangeAnimLeft();
		}
		
	}
	else if (look == Look::UP) {
		position.y -= PacMan_SPEED;
		if (state == State::freeze) WalkUp();
		else
		{
			if (!IsLookingUp()) ChangeAnimUp();
		}

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			position.y = prev_y;
			if (state == State::moving) Stop();
		}
	}
	else if (look == Look::DOWN) {
		position.y += PacMan_SPEED;
		if (state == State::freeze) WalkDown();
		else
		{
			if (!IsLookingDown()) ChangeAnimDown();
		}

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			position.y = prev_y;
			if (state == State::moving) Stop();
		}
	}
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}
void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(position.x, position.y, width, height, col);
	DrawPixel(position.x, position.y-4, WHITE);
}
