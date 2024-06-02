#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include <cmath>

Ghost::Ghost(const Point& p, State2 s, Look2 view, GhostType t) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	mapa = nullptr;
	type = t;
	if (type != GhostType::red) Puerta = true;
	mode = Mode2::Hunt;
	
}
Ghost::~Ghost()
{
}
AppStatus Ghost::Initialise()
{
	const int n = ENEMY_FRAME_SIZE;
	int k;
	switch (type) {
	case GhostType::red:
		k = n * 2;
		break;
	case GhostType::pink:
		k = n;
		break;
	case GhostType::blue:
		k = 0;
		break;
	case GhostType::orange:
		k = n * 3;
		break;
	}

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_Ghost, "resource/Sprites/fantasmas.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	sound_retreat = data.GetSound(AudioResource::retreating);

	render = new Sprite(data.GetTexture(Resource::IMG_Ghost));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)GhostAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)GhostAnim::FREEZE, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::FREEZE, { n*4, (float)k, n, n });

	sprite->SetAnimationDelay((int)GhostAnim::HIDDEN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::HIDDEN, { n * 4, 5*n, n, n });

	sprite->SetAnimationDelay((int)GhostAnim::WALKING_RIGHT, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)GhostAnim::WALKING_RIGHT, { (float)i*n, (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)GhostAnim::WALKING_LEFT, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)GhostAnim::WALKING_LEFT, { 2 * n + ((float)i*n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)GhostAnim::WALKING_UP, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)GhostAnim::WALKING_UP, { 4 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)GhostAnim::WALKING_DOWN, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)GhostAnim::WALKING_DOWN, { 6 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)GhostAnim::PELLET, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)GhostAnim::PELLET, { (float)i * n, 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)GhostAnim::PELLET_FLASH, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)GhostAnim::PELLET_FLASH, { 2*n +((float)i * n), 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)GhostAnim::EYES_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EYES_LEFT, { 0, 5 * n , n, n });

	sprite->SetAnimationDelay((int)GhostAnim::EYES_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EYES_RIGHT, { n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)GhostAnim::EYES_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EYES_UP, { 2*n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)GhostAnim::EYES_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EYES_DOWN, { 3*n, 5 * n , n, n });

	return AppStatus::OK;
}

void Ghost::IntroUpdate(bool turn)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (turn) {
		position.x += ((Ghost_SPEED*2) - 1);
		if (state != State2::PELLET) {
			state = State2::PELLET;
			SetAnimation((int)GhostAnim::PELLET);
		}

		if (introCatch) {
			SetAnimation((int)GhostAnim::HIDDEN);
			state = State2::Moving;
		}
		else {
			if (state == State2::Freeze) StartWalkingRight();
			else {
				if (!IsLookingRight()) SwitchAnimationRight();
			}
		}
	}
	else {
		position.x += -(Ghost_SPEED * 2);
		if (state == State2::Freeze) StartWalkingLeft();
		else {
			if (!IsLookingLeft()) SwitchAnimationLeft();
		}
	}
}

void Ghost::UpdateView(int anim_id)
{
	GhostAnim anim = (GhostAnim)anim_id;
	look = (anim == GhostAnim::WALKING_LEFT) ? Look2::LEFT : Look2::RIGHT;
}

void Ghost::Intro(int count) {
	if (count <= 60) SetAnimation((int)GhostAnim::FREEZE);
	else SetAnimation((int)GhostAnim::HIDDEN);
}

Point Ghost::GetEnemyPos()
{
	return position;
}

void Ghost::SetObjective(Point t)
{
	Objective = t;
}

void Ghost::CancelObjective()
{
	Puerta = true;
	Objective = Leave_Home;
}

void Ghost::SetHome(Point t)
{
	home = t;
}

void Ghost::SetHomeExit(Point t)
{
	Leave_Home = t;
}

bool Ghost::IsDead() {
	return(state == State2::EYES);
}

void Ghost::Pellet(bool ifPellet, int count) {
	if (!Catched) {
		if (ifPellet) {
			state = State2::PELLET;
			vulnearble = true;
			if (count < 300) {
				if (flash) SetAnimation((int)GhostAnim::PELLET_FLASH);
				else SetAnimation((int)GhostAnim::PELLET);
				--delay;
				if (delay <= 0) {
					delay = 30;
					if (flash) flash = false;
					else flash = true;
				}

			}
			else {
				SetAnimation((int)GhostAnim::PELLET);
			}
		}
		else {
			state = State2::Moving;
			switch (look) {
			case Look2::DOWN:
				SwitchAnimationDown();
				break;
			case Look2::UP:
				SwitchAnimationUp();
				break;
			case Look2::RIGHT:
				SwitchAnimationRight();
				break;
			case Look2::LEFT:
				SwitchAnimationLeft();
				break;
			}
			vulnearble = false;
		}
	}
	else {
		if (state != State2::EYES) {
			state = State2::Moving;
			switch (look) {
			case Look2::DOWN:
				SwitchAnimationDown();
				break;
			case Look2::UP:
				SwitchAnimationUp();
				break;
			case Look2::RIGHT:
				SwitchAnimationRight();
				break;
			case Look2::LEFT:
				SwitchAnimationLeft();
				break;
			}
			vulnearble = false;
		}
	}
}

void Ghost::WinLose() {
	SetAnimation((int)GhostAnim::HIDDEN);
}

void Ghost::SetTileMap(TileMap* tilemap)
{
	mapa = tilemap;
}

bool Ghost::IsLookingRight() const
{
	return look == Look2::RIGHT;
}
bool Ghost::IsLookingLeft() const
{
	return look == Look2::LEFT;
}
bool Ghost::IsLookingUp() const
{
	return look == Look2::UP;
}
bool Ghost::IsLookingDown() const
{
	return look == Look2::DOWN;
}
void Ghost::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
GhostAnim Ghost::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (GhostAnim)sprite->GetAnimation();
}
void Ghost::Stop()
{
	dir = { 0,0 };
	if (state != State2::PELLET) {
		state = State2::Freeze;
		SetAnimation((int)GhostAnim::FREEZE);
	}
}
void Ghost::StartWalkingLeft()
{
	look = Look2::LEFT;
	if (state != State2::PELLET) {
		state = State2::Moving;
		SetAnimation((int)GhostAnim::WALKING_LEFT);
	}
}
void Ghost::StartWalkingRight()
{
	look = Look2::RIGHT;
	if (state != State2::PELLET) {
		state = State2::Moving;
		SetAnimation((int)GhostAnim::WALKING_RIGHT);
	}
}
void Ghost::StartWalkingUp()
{
	look = Look2::UP;
	if (state != State2::PELLET) {
		state = State2::Moving;
		SetAnimation((int)GhostAnim::WALKING_UP);
	}
}
void Ghost::StartWalkingDown()
{
	look = Look2::DOWN;
	if (state != State2::PELLET) {
		state = State2::Moving;
		SetAnimation((int)GhostAnim::WALKING_DOWN);
	}
}
void Ghost::StartDying()
{
	state = State2::EYES;

	//SetAnimation((int)EnemyAnim::PELLET);
}
void Ghost::SwitchAnimationRight()
{
	look = Look2::RIGHT;
	switch (state)
	{
	case State2::Freeze:	 SetAnimation((int)GhostAnim::FREEZE);    break;
	case State2::Moving: SetAnimation((int)GhostAnim::WALKING_RIGHT); break;
	case State2::PELLET: SetAnimation((int)GhostAnim::PELLET); break;
	case State2::EYES: SetAnimation((int)GhostAnim::EYES_RIGHT); break;
	}
}
void Ghost::SwitchAnimationLeft()
{
	look = Look2::LEFT;
	switch (state)
	{
	case State2::Freeze:	 SetAnimation((int)GhostAnim::FREEZE);    break;
	case State2::Moving: SetAnimation((int)GhostAnim::WALKING_LEFT); break;
	case State2::PELLET: SetAnimation((int)GhostAnim::PELLET); break;
	case State2::EYES: SetAnimation((int)GhostAnim::EYES_LEFT); break;
	}
}
void Ghost::SwitchAnimationUp()
{
	look = Look2::UP;
	switch (state)
	{
	case State2::Freeze:	 SetAnimation((int)GhostAnim::FREEZE);    break;
	case State2::Moving: SetAnimation((int)GhostAnim::WALKING_UP); break;
	case State2::PELLET: SetAnimation((int)GhostAnim::PELLET); break;
	case State2::EYES: SetAnimation((int)GhostAnim::EYES_UP); break;
	}
}
void Ghost::SwitchAnimationDown()
{
	look = Look2::DOWN;
	switch (state)
	{
	case State2::Freeze:	 SetAnimation((int)GhostAnim::FREEZE);    break;
	case State2::Moving: SetAnimation((int)GhostAnim::WALKING_DOWN); break;
	case State2::PELLET: SetAnimation((int)GhostAnim::PELLET); break;
	case State2::EYES: SetAnimation((int)GhostAnim::EYES_DOWN); break;
	}
}
void Ghost::Update(Point pacmanDir, Point pacmanPos, Point blinkypos)
{
	//all movement in move
	Move(pacmanDir, pacmanPos, blinkypos);

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
float Ghost::GetTargetDistance(Point dir)
{
	Point check = position;
	check.x += (dir.x * Ghost_SPEED);
	check.y += (dir.y * Ghost_SPEED);
	return static_cast<float>(sqrt(pow(check.x - Objective.x, 2) + pow(check.y - Objective.y, 2)));
}
void Ghost::Move(Point pacmanDir, Point pacmanPos, Point blinkypos)
{
	AABB box;
	int prev_x = position.x;
	int prev_y = position.y;
	int availableWays = 0;
	Point optimalWay;
	Look2 backwards;

	if (Catched) {
		if (state != State2::EYES) {
			state = State2::EYES;
			switch (look) {
			case Look2::LEFT:
				SwitchAnimationLeft();
				break;
			case Look2::RIGHT:
				SwitchAnimationRight();
				break;
			case Look2::UP:
				SwitchAnimationUp();
				break;
			case Look2::DOWN:
				SwitchAnimationDown();
				break;
			}
		}
		if (home.x == position.x and home.y == position.y) {
			Catched = false;
			if (IsSoundPlaying(sound_retreat)) StopSound(sound_retreat);
			Puerta = false;
			Objective = Leave_Home;
			state = State2::Moving;
			switch (look) {
			case Look2::LEFT:
				SwitchAnimationLeft();
				break;
			case Look2::RIGHT:
				SwitchAnimationRight();
				break;
			case Look2::UP:
				SwitchAnimationUp();
				break;
			case Look2::DOWN:
				SwitchAnimationDown();
				break;
			}
		}
		Objective = home;
		if (!IsSoundPlaying(sound_retreat)) PlaySound(sound_retreat);
	} 
	
	UpdateTarget(pacmanDir, pacmanPos, blinkypos);

	if (pacmanPos.x <= (position.x + TILE_SIZE * 12) and pacmanPos.x >= (position.x - TILE_SIZE * 12)) {
		if (pacmanPos.y <= (position.y + TILE_SIZE * 12) and pacmanPos.y >= (position.y - TILE_SIZE * 12)) mode = Mode2::Hunt;
		else mode = Mode2::Scared;
	}
	else mode = Mode2::Scared;

	if (state != State2::PELLET) {
		Point direction;
		Look2 checking;

		switch (look) {
		case Look2::LEFT:
			optimalWay = { -1, 0 };
			backwards = Look2::RIGHT;
			break;
		case Look2::RIGHT:
			optimalWay = { 1, 0 };
			backwards = Look2::LEFT;
			break;
		case Look2::UP:
			optimalWay = { 0, -1 };
			backwards = Look2::DOWN;
			break;
		case Look2::DOWN:
			optimalWay = { 0, 1 };
			backwards = Look2::UP;
			break;
		}

		for (int i = 0; i < 4; ++i) {
			if (i != (int)backwards) {
				checking = (Look2)i;
				bool possible = false;
				switch (checking) {
				case Look2::LEFT:
					position.x += -Ghost_SPEED;
					box = GetHitbox();
					if (!mapa->TestCollisionWallLeft(box)) {
						availableWays++;
						possible = true;
					}
					position.x = prev_x;
					direction = { -1, 0 };
					break;
				case Look2::RIGHT:
					position.x += Ghost_SPEED;
					box = GetHitbox();
					if (!mapa->TestCollisionWallRight(box)) {
						availableWays++;
						possible = true;
					}
					position.x = prev_x;
					direction = { 1, 0 };
					break;
				case Look2::UP:
					position.y -= Ghost_SPEED;
					box = GetHitbox();
					if (!mapa->TestCollisionWallUp(box, Puerta)) {
						availableWays++;
						possible = true;
					}
					position.y = prev_y;
					direction = { 0, -1 };
					break;
				case Look2::DOWN:
					position.y += Ghost_SPEED;
					box = GetHitbox();
					if (!mapa->TestCollisionWallDown(box, Puerta)) {
						availableWays++;
						possible = true;
					}
					position.y = prev_y;
					direction = { 0, 1 };
					break;
				}
				if (possible) {
					if (GetTargetDistance(direction) < GetTargetDistance(optimalWay)) optimalWay = direction;
				}
			}
		}

		if (availableWays > 0) {
			if (optimalWay.x == 0) {
				if (optimalWay.y == 1) {
					position.y += Ghost_SPEED;
					if (state == State2::Freeze) StartWalkingDown();
					else {
						if (!IsLookingDown()) SwitchAnimationDown();
					}
				}
				else {
					position.y -= Ghost_SPEED;
					if (state == State2::Freeze) StartWalkingUp();
					else {
						if (!IsLookingUp()) SwitchAnimationUp();
					}
				}
			}
			else {
				if (optimalWay.x == 1) {
					position.x += Ghost_SPEED;
					if (state == State2::Freeze) StartWalkingRight();
					else {
						if (!IsLookingRight()) SwitchAnimationRight();
					}
					if (position.x == WINDOW_WIDTH - 9) {
						position.x = 1;
						SwitchAnimationLeft();
					}
				}
				else {
					position.x += -Ghost_SPEED;
					if (state == State2::Freeze) StartWalkingLeft();
					else {
						if (!IsLookingLeft()) SwitchAnimationLeft();
					}
					if (position.x == 1) {
						position.x = WINDOW_WIDTH - 9;
						SwitchAnimationRight();
					}
				}
			}
		}
		else {
			switch (backwards) {
			case Look2::LEFT:
				position.x += -Ghost_SPEED;
				if (state == State2::Freeze) StartWalkingLeft();
				else {
					if (!IsLookingLeft()) SwitchAnimationLeft();
				}
				break;
			case Look2::RIGHT:
				position.x += Ghost_SPEED;
				if (state == State2::Freeze) StartWalkingRight();
				else {
					if (!IsLookingRight()) SwitchAnimationRight();
				}
				break;
			case Look2::UP:
				position.y += -Ghost_SPEED;
				if (state == State2::Freeze) StartWalkingUp();
				else {
					if (!IsLookingUp()) SwitchAnimationUp();
				}
				break;
			case Look2::DOWN:
				position.y += Ghost_SPEED;
				if (state == State2::Freeze) StartWalkingDown();
				else {
					if (!IsLookingDown()) SwitchAnimationDown();
				}
				break;
			}
		}
	}
 else {
	 bool canMove = false;
	 bool notFound = false;
	 std::vector<Look2> possible;
	 Look2 targetDir;
	 int random;
	 int i = 0;

	 switch (look) {
	 case Look2::LEFT:
		 optimalWay = { -1, 0 };
		 backwards = Look2::RIGHT;
		 break;
	 case Look2::RIGHT:
		 optimalWay = { 1, 0 };
		 backwards = Look2::LEFT;
		 break;
	 case Look2::UP:
		 optimalWay = { 0, -1 };
		 backwards = Look2::DOWN;
		 break;
	 case Look2::DOWN:
		 optimalWay = { 0, 1 };
		 backwards = Look2::UP;
		 break;
	 }

	 for (int i = 0; i < 4; ++i) {
		 if (i != (int)backwards) {
			 targetDir = (Look2)i;
			 switch (targetDir) {
			 case Look2::LEFT:
				 position.x += -(Ghost_SPEED + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallLeft(box)) possible.push_back(targetDir);
				 position.x = prev_x;
				 break;
			 case Look2::RIGHT:
				 position.x += (Ghost_SPEED + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallRight(box)) possible.push_back(targetDir);
				 position.x = prev_x;
				 break;
			 case Look2::UP:
				 position.y += -(Ghost_SPEED + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallUp(box, Puerta)) possible.push_back(targetDir);
				 position.y = prev_y;
				 break;
			 case Look2::DOWN:
				 position.y += (Ghost_SPEED + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallDown(box, Puerta)) possible.push_back(targetDir);
				 position.y = prev_y;
				 break;
			 }
		 }
	 }

	 if (possible.size() > 0) {
		 random = GetRandomValue(0, possible.size() - 1);
		 targetDir = possible[random];
	 }
	 else targetDir = backwards;
	 
	 switch (targetDir) {
	 case Look2::LEFT:
		 position.x += -(Ghost_SPEED + 1);
		 if (state == State2::Freeze) StartWalkingLeft();
		 else {
			 if (!IsLookingLeft()) SwitchAnimationLeft();
		 }
		 if (position.x == 1) {
			 position.x = WINDOW_WIDTH - 9;
			 SwitchAnimationRight();
		 }
		 break;
	 case Look2::RIGHT:
		 position.x += (Ghost_SPEED + 1);
		 if (state == State2::Freeze) StartWalkingRight();
		 else {
			 if (!IsLookingRight()) SwitchAnimationRight();
		 }
		 if (position.x == WINDOW_WIDTH - 9) {
			 position.x = 1;
			 SwitchAnimationLeft();
		 }
		 break;
	 case Look2::UP:
		 position.y += -(Ghost_SPEED + 1);
		 if (state == State2::Freeze) StartWalkingUp();
		 else {
			 if (!IsLookingUp()) SwitchAnimationUp();
		 }
		 break;
	 case Look2::DOWN:
		 position.y += (Ghost_SPEED + 1);
		 if (state == State2::Freeze) StartWalkingDown();
		 else {
			 if (!IsLookingDown()) SwitchAnimationDown();
		 }
		 break;
	 }
}
 
}

void Ghost::UpdateTarget(Point PacDir, Point PosPacman, Point RedPos)
{
	if (Puerta) {
		if (position.x == Objective.x and position.y == Objective.y) Puerta = false;
		else if (home.x == Objective.x and home.y == Objective.y) {
			state = State2::Freeze;
			Catched = false;
			Objective = Leave_Home;
		}
	}
	else if(state != State2::EYES){
		if (mode == Mode2::Scared) {
			switch (type) {
			case GhostType::red:
				Objective = { TILE_SIZE * (LEVEL_WIDTH - 1), 0 };
				break;
			case GhostType::pink:
				Objective = { 0,0 };
				break;
			case GhostType::blue:
				Objective = { TILE_SIZE * (LEVEL_WIDTH - 1), TILE_SIZE * (LEVEL_HEIGHT - 1) };
				break;
			case GhostType::orange:
				Objective = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
				break;
			}
		}
		else {
			switch (type) {
			case GhostType::red:
				Objective = PosPacman;
				break;
			case GhostType::pink:
				Objective = PosPacman + (PacDir * TILE_SIZE * 4);
				break;
			case GhostType::blue:
				Objective = PosPacman + (PacDir * TILE_SIZE * 2);
				Objective.x += Objective.x - RedPos.x;
				Objective.y += Objective.y - RedPos.y;
				break;
			case GhostType::orange:
				if (PosPacman.x <= (position.x + TILE_SIZE * 6) and PosPacman.x >= (position.x - TILE_SIZE * 6)) {
					if (PosPacman.y <= (position.y + TILE_SIZE * 6) and PosPacman.y >= (position.y - TILE_SIZE * 6)) Objective = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
					else Objective = PosPacman;
				}
				else Objective = PosPacman;
				break;
			}
		}
	}
}

void Ghost::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(position.x, position.y, width, height, col);
}


void Ghost::Release()
{
	if (IsSoundPlaying(sound_retreat)) StopSound(sound_retreat);
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_Ghost);

	render->Release();
}