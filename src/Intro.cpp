#include "Intro.h"
#include <stdio.h>
#include "Globals.h"

Intro::Intro() 
{
	PacMan = nullptr;
	Red = nullptr;
	Pink = nullptr;
	Orange = nullptr;
	Blue = nullptr;
	Dot = nullptr;
	introScene = nullptr;

	camera.target = { 0, 0 };				
	camera.offset = { 0, 0 };	
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

Intro::~Intro()
{
	if (PacMan != nullptr) {
		PacMan->Release();
		delete PacMan;
		PacMan = nullptr;
	}
	if (Blue != nullptr) {
		Blue->Release();
		delete Blue;
		Blue = nullptr;
	}
	if (Pink != nullptr) {
		Pink->Release();
		delete Pink;
		Pink = nullptr;
	}
	if (Red != nullptr) {
		Red->Release();
		delete Red;
		Red = nullptr;
	}
	
	if (Orange != nullptr) {
		Orange->Release();
		delete Orange;
		Orange = nullptr;
	}
	if (introScene != nullptr) {
		introScene->Release();
		delete introScene;
		introScene = nullptr;
	}
	if (Dot != nullptr) {
		delete Dot;
		Dot = nullptr;
	}
	
}

AppStatus Intro::Init() 
{
	PacMan = new Player({ 0,0 }, State::freeze, Look::LEFT);
	if (PacMan == nullptr)
	{
		LOG("Failed to allocate memory for intro PacMan");
		return AppStatus::ERROR;
	}
	Red = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::red);
	if (Red == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Blue = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::blue);
	if (Blue == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	
	Pink = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::pink);
	if (Pink == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Orange = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::orange);
	if (Orange == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}

	if(PacMan->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro pacman");
		return AppStatus::ERROR;
	}
	if (Orange->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (Blue->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (Red->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	
	if (Pink->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}

	introScene = new TileMap();
	if (introScene == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	if (introScene->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (LoadIntro() != AppStatus::OK)
	{
		LOG("Failed to load intro");
		return AppStatus::ERROR;
	}
	
	PacMan->SetTileMap(introScene);

	return AppStatus::OK;
}

AppStatus Intro::LoadIntro() 
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* mapa = nullptr;
	size = LEVEL_WIDTH * LEVEL_HEIGHT;

	mapa = new int[size] {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)mapa[i];
			if (tile == Tile::EMPTY)
			{
				mapa[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				pos.x = x * TILE_SIZE;
				dotX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				dotY = pos.y;
				Dot = new Object(pos, ObjectType::PELLET);
				mapa[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				pacmanX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				pacmanY = pos.y;
				PacMan->SetPos(pos);
				mapa[i] = 0;
			}
			
			++i;
		}
	}
	
	introScene->Load(mapa, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] mapa;
	return AppStatus::OK;
}

void Intro::Update() 
{

	if (end) {
		turn = false;
		PacMan->SetPos({ pacmanX, pacmanY });
		Dot = new Object({dotX, dotY}, ObjectType::PELLET);
		isDot = true;

		Orange->introCatch = false;
		isorangeGhost = false;
		Red->introCatch = false;
		isredGhost = false;
		Pink->introCatch = false;
		ispinkGhost = false;
		Blue->introCatch = false;
		isblueGhost = false;
		

		end = false;
		timer = 0;

		if (loop) {
			loop = false;
			loopCheck = true;
		}
	}
	else {
		Point p1, p2;
		AABB box;

		if (timer == 10) {
			Red->SetPos({ pacmanX, pacmanY });
			isredGhost = true;
		}
		if (timer == 30) {
			Blue->SetPos({ pacmanX, pacmanY });
			isblueGhost = true;
		}
		if (timer == 20) {
			Pink->SetPos({ pacmanX, pacmanY });
			ispinkGhost = true;
		}
		
		if (timer == 40) {
			Orange->SetPos({ pacmanX, pacmanY });
			isorangeGhost = true;
		}

		end = PacMan->IntroUpdate(turn);
		if (end) loop = true;

		if (isredGhost) Red->IntroUpdate(turn);
		if (ispinkGhost) Pink->IntroUpdate(turn);
		if (isorangeGhost) Orange->IntroUpdate(turn);
		if (isblueGhost) Blue->IntroUpdate(turn);
		

		CheckCollisions();
		timer++;
	}
}

void Intro::Render() 
{
	BeginMode2D(camera);
	introScene->Render();
	PacMan->DrawPlayer();
	Red->DrawPlayer();
	Pink->DrawPlayer();
	Blue->DrawPlayer();
	
	Orange->DrawPlayer();
	if(isDot)Dot->Draw();
	EndMode2D();
}

void Intro::CheckCollisions()
{
	AABB PacMan_box, obj_box, Ghost_box;

	PacMan_box = PacMan->GetHitbox();

	if (isDot) {
		obj_box = Dot->GetHitbox();
		if (PacMan_box.TestAABB(obj_box)) {
			delete Dot;
			turn = true;
			isDot = false;
		}
	}

	if (isorangeGhost) {
		Ghost_box = Orange->GetHitbox();
		if (PacMan_box.TestAABB(Ghost_box)) Orange->introCatch = true;
	}
	if (isredGhost) {
		Ghost_box = Red->GetHitbox();
		if (PacMan_box.TestAABB(Ghost_box)) Red->introCatch = true;
	}
	if (ispinkGhost) {
		Ghost_box = Pink->GetHitbox();
		if (PacMan_box.TestAABB(Ghost_box)) Pink->introCatch = true;
	}
	if (isblueGhost) {
		Ghost_box = Blue->GetHitbox();
		if (PacMan_box.TestAABB(Ghost_box)) Blue->introCatch = true;
	}

	
}

void Intro::Release() 
{
	Orange->Release();
	introScene->Release();
	PacMan->Release();
	Red->Release();
	Blue->Release();
	Pink->Release();
	
}