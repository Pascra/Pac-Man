#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;

	redGhost = nullptr;
	blueGhost = nullptr;
	orangeGhost = nullptr;
	pinkGhost = nullptr;

    level = nullptr;

	livesUI = nullptr;

	font = nullptr;
	
	camera.target = { 0, 0 };	//centre pantalla
	camera.offset = { 0, 0 };	//Offset del targuet
	camera.rotation = 0.0f;	 //No rotacio
	camera.zoom = 1.0f;		// zoom Normal

	ResourceManager& data = ResourceManager::Instance();
	sound_intro = data.GetSound(AudioResource::INtro);

	sound_munch1 = data.GetSound(AudioResource::munch_1);
	sound_munch2 = data.GetSound(AudioResource::munch_2);

	sound_pellet = data.GetSound(AudioResource::PowerPellet);
	sound_fruit = data.GetSound(AudioResource::eatFruit);
	sound_eatghost = data.GetSound(AudioResource::KillGhost);

	sirens[0] = data.GetSound(AudioResource::SIREN1);
	sirens[1] = data.GetSound(AudioResource::SIREN2);
	sirens[2] = data.GetSound(AudioResource::SIREN3);
	sirens[3] = data.GetSound(AudioResource::SIREN4);
	sirens[4] = data.GetSound(AudioResource::SIREN5);

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	StopSound(sirens[siren]);
	if(IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);

	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (redGhost != nullptr)
	{
		redGhost->Release();
		delete redGhost;
		redGhost = nullptr;
	}
	if (orangeGhost != nullptr)
	{
		orangeGhost->Release();
		delete orangeGhost;
		orangeGhost = nullptr;
	}
	if (blueGhost != nullptr)
	{
		blueGhost->Release();
		delete blueGhost;
		blueGhost = nullptr;
	}
	if (pinkGhost != nullptr)
	{
		pinkGhost->Release();
		delete pinkGhost;
		pinkGhost = nullptr;
	}
	if (font != nullptr) {
		delete font;
		font = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	if (livesUI != nullptr) {
		livesUI->Release();
		delete livesUI;
		livesUI = nullptr;
	}
	
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{

	//Crear jugador
	player = new Player({ 0,0 }, State::freeze, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//crear fantasma azul
	blueGhost = new Ghost({0,0}, State2::Freeze, Look2::RIGHT, GhostType::blue);
	if (blueGhost == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	//crear fantasma rojo
	redGhost = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::red);
	if (redGhost == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	//crear fantasma rosa
	pinkGhost = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::pink);
	if (pinkGhost == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	//crear fantasma naranja
	orangeGhost = new Ghost({ 0,0 }, State2::Freeze, Look2::LEFT, GhostType::orange);
	if (orangeGhost == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	//crear UI
	livesUI = new UI({10, (WINDOW_HEIGHT)});

	//Iniciar jugador
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	//Iniciar fantasmas

	if (blueGhost->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (pinkGhost->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (redGhost->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	
	if (orangeGhost->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	//Iniciar UI
	if (livesUI->Initialise() != AppStatus::OK) {
		LOG("Failed to initialise lives UI");
		return AppStatus::ERROR;
	}

	//crear nivel
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Iniciar nivel
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//cargar nivel
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}

	player->SetTileMap(level);
	blueGhost->SetTileMap(level);
	orangeGhost->SetTileMap(level);
	pinkGhost->SetTileMap(level);
	redGhost->SetTileMap(level);
	PlaySound(sound_intro);

	font = new Text();
	if (font->Initialise(Resource::IMG_FONT, "resource/Sprites/letras.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (font == nullptr) {
		LOG("Failed to allocate memory for font");
		return AppStatus::ERROR;
	}
	

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	Object *obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 0)
	{
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 48, 47, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1,
				4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
				4, 0, 40, 15, 15, 39, 0, 40, 15, 15, 15, 39, 0, 25, 26, 0, 40, 15, 15, 15, 39, 0, 40, 15, 15, 39, 0, 3,
				4, 0, 25, 0, 0, 26, 0, 25, 0, 0, 0, 26, 0, 25, 26, 0, 25, 0, 0, 0, 26, 0, 25, 0, 0, 26, 0, 3,
				4, 0, 28, 21, 21, 27, 0, 28, 21, 21, 21, 27, 0, 37, 38, 0, 28, 21, 21, 21, 27, 0, 28, 21, 21, 27, 0, 3,
				4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
				4, 0, 40, 15, 15, 39, 0, 35, 36, 0, 40, 15, 15, 15, 15, 15, 15, 39, 0, 35, 36, 0, 40, 15, 15, 39, 0, 3,
				4, 0, 28, 21, 21, 27, 0, 25, 26, 0, 28, 21, 21, 45, 46, 21, 21, 27, 0, 25, 26, 0, 28, 21, 21, 27, 0, 3,
				4, 0, 0, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 0, 0, 3,
				6, 13, 13, 13, 13, 23, 0, 25, 28, 15, 15, 39, 0, 25, 26, 0, 40, 15, 15, 27, 26, 0, 24, 13, 13, 13, 13, 5,
				0, 0, 0, 0, 0, 4, 0, 25, 40, 21, 21, 27, 0, 37, 38, 0, 28, 21, 21, 39, 26, 0, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 0, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 26, 0, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 0, 25, 26, 0, 30, 13, 34, 70, 71, 33, 13, 29, 0, 25, 26, 0, 3, 0, 0, 0, 0, 0,
				11, 11, 11, 11, 11, 27, 0, 37, 38, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 37, 38, 0, 28, 11, 11, 11, 11, 11,
				-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2,
				13, 13, 13, 13, 13, 23, 0, 35, 36, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 35, 36, 0, 24, 13, 13, 13, 13, 13,
				0, 0, 0, 0, 0, 4, 0, 25, 26, 0, 32, 11, 11, 11, 11, 11, 11, 31, 0, 25, 26, 0, 3, 0, 0, 0, 0, 0,
				0, 0, 101, 0, 0, 4, 0, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 26, 0, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 0, 25, 26, 0, 40, 15, 15, 15, 15, 15, 15, 39, 0, 25, 26, 0, 3, 0, 0, 0, 0, 0,
				2, 11, 11, 11, 11, 27, 0, 37, 38, 0, 28, 21, 21, 45, 46, 21, 21, 27, 0, 37, 38, 0, 28, 11, 11, 11, 11, 1,
				4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
				4, 0, 40, 15, 15, 39, 0, 40, 15, 15, 15, 39, 0, 25, 26, 0, 40, 15, 15, 15, 39, 0, 40, 15, 15, 39, 0, 3,
				4, 0, 28, 21, 39, 26, 0, 28, 21, 21, 21, 27, 0, 37, 38, 0, 28, 21, 21, 21, 27, 0, 25, 40, 21, 27, 0, 3,
				4, 0, 0, 0, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 26, 0, 0, 0, 3,
				8, 15, 39, 0, 25, 26, 0, 35, 36, 0, 40, 15, 15, 15, 15, 15, 15, 39, 0, 35, 36, 0, 25, 26, 0, 40, 15, 7,
				10, 21, 27, 0, 37, 38, 0, 25, 26, 0, 28, 21, 21, 45, 46, 21, 21, 27, 0, 25, 26, 0, 37, 38, 0, 28, 21, 9,
				4, 0, 0, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 0, 0, 3,
				4, 0, 40, 15, 15, 15, 15, 27, 28, 15, 15, 39, 0, 25, 26, 0, 40, 15, 15, 27, 28, 15, 15, 15, 15, 39, 0, 3,
				4, 0, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 0, 37, 38, 0, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 0, 3,
				4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
				6, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 5,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
	}
	else if (stage == 1 or stage == 2)
	{
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 48, 47, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1,
				4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
				4, 50, 40, 15, 15, 39, 50, 40, 15, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 15, 39, 50, 40, 15, 15, 39, 50, 3,
				4, 52, 25, 0, 0, 26, 50, 25, 0, 0, 0, 26, 50, 25, 26, 50, 25, 0, 0, 0, 26, 50, 25, 0, 0, 26, 52, 3,
				4, 50, 28, 21, 21, 27, 50, 28, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 27, 50, 28, 21, 21, 27, 50, 3,
				4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
				4, 50, 40, 15, 15, 39, 50, 35, 36, 50, 40, 15, 15, 15, 15, 15, 15, 39, 50, 35, 36, 50, 40, 15, 15, 39, 50, 3,
				4, 50, 28, 21, 21, 27, 50, 25, 26, 50, 28, 21, 21, 45, 46, 21, 21, 27, 50, 25, 26, 50, 28, 21, 21, 27, 50, 3,
				4, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 3,
				6, 13, 13, 13, 13, 23, 50, 25, 28, 15, 15, 39, 0, 25, 26, 0, 40, 15, 15, 27, 26, 50, 24, 13, 13, 13, 13, 5,
				0, 0, 0, 0, 0, 4, 50, 25, 40, 21, 21, 27, 0, 37, 38, 0, 28, 21, 21, 39, 26, 50, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 50, 25, 26, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 25, 26, 50, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 50, 25, 26, 0, 30, 13, 34, 70, 71, 33, 13, 29, 0, 25, 26, 50, 3, 0, 0, 0, 0, 0,
				11, 11, 11, 11, 11, 27, 50, 37, 38, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 37, 38, 50, 28, 11, 11, 11, 11, 11,
				-3, 0, 0, 0, 0, 0, 50, 0, 0, 0, 3, 103, 0, 102, 0, 0, 104, 4, 0, 0, 0, 50, 0, 0, 0, 0, 0, -2,
				13, 13, 13, 13, 13, 23, 50, 35, 36, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 35, 36, 50, 24, 13, 13, 13, 13, 13,
				0, 0, 0, 0, 0, 4, 50, 25, 26, 0, 32, 11, 11, 11, 11, 11, 11, 31, 0, 25, 26, 50, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 50, 25, 26, 0, 0, 0, 0, 53, 0, 0, 0, 0, 0, 25, 26, 50, 3, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 4, 50, 25, 26, 0, 40, 15, 15, 15, 15, 15, 15, 39, 0, 25, 26, 50, 3, 0, 0, 0, 0, 0,
				2, 11, 11, 11, 11, 27, 50, 37, 38, 0, 28, 21, 21, 45, 46, 21, 21, 27, 0, 37, 38, 50, 28, 11, 11, 11, 11, 1,
				4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
				4, 50, 40, 15, 15, 39, 50, 40, 15, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 15, 39, 50, 40, 15, 15, 39, 50, 3,
				4, 50, 28, 21, 39, 26, 50, 28, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 27, 50, 25, 40, 21, 27, 50, 3,
				4, 52, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 100, 0, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 52, 3,
				8, 15, 39, 50, 25, 26, 50, 35, 36, 50, 40, 15, 15, 15, 15, 15, 15, 39, 50, 35, 36, 50, 25, 26, 50, 40, 15, 7,
				10, 21, 27, 50, 37, 38, 50, 25, 26, 50, 28, 21, 21, 45, 46, 21, 21, 27, 50, 25, 26, 50, 37, 38, 50, 28, 21, 9,
				4, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 3,
				4, 50, 40, 15, 15, 15, 15, 27, 28, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 27, 28, 15, 15, 15, 15, 39, 50, 3,
				4, 50, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 50, 3,
				4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
				6, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 5,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		if(stage == 1) player->StartScore();
		if (stage == 2) levelintro = true;
		siren = 0;
		fruitcounter = FRUITTIME;
		collectPellet = false;
		contador_pellet = PELLETTemps;
		blueGhost->CancelObjective();
		if (IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::Blue_Ghost)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				AzulX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				AzulY = pos.y;
				blueGhost->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::Red_Ghost)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				RedX = pos.x;
				RedY = pos.y;
				redGhost->SetPos(pos);
				redGhost->SetHome(pos);
				pinkGhost->SetHome(pos);
				orangeGhost->SetHome(pos);
				blueGhost->SetHome(pos);
				map[i] = 0;
			}
			else if (tile == Tile::Pink_Ghost)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				PinkX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				PinkY = pos.y;
				pinkGhost->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::Orange_Ghost)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				OrangeX = pos.x;
				OrangeY = pos.y;
				orangeGhost->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				pacmanX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				pacmanY = pos.y;
				player->SetPos(pos);
				map[i] = 0;
			}
			
			else if (tile == Tile::DOT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::DOT);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::PELLET);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::FRUIT)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				fruitX = pos.x;
				fruitY = pos.y;
				map[i] = 0;
			}
			else if (tile == Tile::FRUIT_ICON)
			{
				if(level_count == 1) map[i] = (int)Tile::FRUIT_ICON_1;
				if (level_count == 2) map[i] = (int)Tile::FRUIT_ICON_2;
			}
			else if (tile == Tile::GHOST_DOOR) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				redGhost->SetObjective(pos);
				pinkGhost->SetObjective(pos);
				orangeGhost->SetObjective(pos);

				pinkGhost->SetHomeExit(pos);
				orangeGhost->SetHomeExit(pos);
				redGhost->SetHomeExit(pos);
				blueGhost->SetHomeExit(pos);
				
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	delete[] map;

	return AppStatus::OK;
}
void Scene::Update()
{
	

	Point p1, p2;
	AABB box;

	//mirar hitboxes
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Ganar/Perder/spawnear items
	if (IsKeyPressed(KEY_F2))       EndLevel = true;
	if (IsKeyPressed(KEY_F3))       lose = true;
	if (IsKeyPressed(KEY_F5)) {
		int objectX, objectY;
		bool checkTile = true;
		while (checkTile) {
			objectX = (int)GetRandomValue(7, LEVEL_WIDTH - 7);
			objectY = (int)GetRandomValue(7, LEVEL_HEIGHT - 7);
			AABB box({ objectX, objectY }, TILE_SIZE, TILE_SIZE);
			if (objectX <= 16 and objectX >= 11) {
				if (objectY >= 16 and objectY <= 18) {
					checkTile = true;
				}
				else {
					if (!level->SolidTest(box)) checkTile = false;
				}
			}
			else {
				if (!level->SolidTest(box)) checkTile = false;
			}
		}
		objectX = objectX * TILE_SIZE;
		objectY = objectY * TILE_SIZE + TILE_SIZE - 1;
		Object* obj = new Object({objectX, objectY}, level_count);
		objects.push_back(obj);
	}

	//godmode
	if (IsKeyPressed(KEY_F4)) {
		if (god_mode) god_mode = false;
		else god_mode = true;
	}

	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE)) {
		level_count = 1;
		LoadLevel(1);
	}
	if (IsKeyPressed(KEY_TWO)) {
		level_count = 2;
		LoadLevel(2);
	}

	if (EndLevel) {
		StopSound(sirens[siren]);
		collectPellet = false;
		contador_pellet = PELLETTemps;
		if(IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);

		level->win = true;
		player->Win();
		redGhost->WinLose();
		pinkGhost->WinLose();
		blueGhost->WinLose();
		orangeGhost->WinLose();
		win = true;

		LoadLevel(0);
		EndLevel = false;
	}

	if (fruitcounter == 0) {
		Object* obj = new Object({ fruitX, fruitY }, level_count);
		objects.push_back(obj);
	}
	fruitcounter--;

	if (intro or levelintro) 
	{
		if (intro_count <= 0) {
			if (intro) player->setLives(2);
			if(intro) intro = false;
			if (levelintro) levelintro = false;
			intro_count = 240;
		}
		else 
		{
			player->Intro(intro_count);
			redGhost->Intro(intro_count);
			orangeGhost->Intro(intro_count);
			blueGhost->Intro(intro_count);
			pinkGhost->Intro(intro_count);
			--intro_count;
		}
	}
	else if (win) {
		if (!level->win) {
			level_count++;
			win = false;
			if (level_count > (int)LEVELS) {
				EndGame = true;
			}
			else {
				LoadLevel(level_count);
			}
		}
	}
	else if (lose) {
		StopSound(sirens[siren]);
		player->Defeat();
		if (!player->lose) {
			lose = false;
			if (player->AddLives() >= 0) {
				player->SetPos({ pacmanX, pacmanY });
				pinkGhost->SetPos({ PinkX, PinkY });
				orangeGhost->SetPos({ OrangeX, OrangeY });
				blueGhost->SetPos({ AzulX, AzulY });
				redGhost->SetPos({ RedX, RedY });
				
			}
			else {
				collectPellet = false;
				contador_pellet = PELLETTemps;
				if (IsSoundPlaying(sound_pellet)) StopSound(sound_pellet); StopSound(sound_pellet);
				EndGame = true;
			}
		}
	}
	else {
		//background siren
		if (!IsSoundPlaying(sirens[siren])) {
			if (siren != 0) {
				if (!IsSoundPlaying(sirens[siren - 1])) PlaySound(sirens[siren]);
			} else PlaySound(sirens[siren]);
		}

		if (collectPellet) {

			
			if (RedCaught and BlueCaught and OrangeCaught and PinkCaught) {
				OrangeCaught = false;
				RedCaught = false;
				BlueCaught = false;
				PinkCaught = false;
				collectPellet = false;
				ghost_points = 200;
				contador_pellet = PELLETTemps;
				StopSound(sound_pellet);
			}
			if (contador_pellet >= 0) {
				if (!IsSoundPlaying(sound_pellet)) PlaySound(sound_pellet);
				--contador_pellet;
			}
			
			else {
				OrangeCaught = false;
				RedCaught = false;
				BlueCaught = false;
				PinkCaught = false;
				collectPellet = false;
				ghost_points = 200;
				contador_pellet = PELLETTemps;
				StopSound(sound_pellet);
			}
			if(!RedCaught) redGhost->Pellet(collectPellet, contador_pellet);
			if(!BlueCaught) blueGhost->Pellet(collectPellet, contador_pellet);
			if(!OrangeCaught) orangeGhost->Pellet(collectPellet, contador_pellet);
			if(!PinkCaught) pinkGhost->Pellet(collectPellet, contador_pellet);
		}

		level->Update();
		player->Update();
		blueGhost->Update(player->GetDirection(), player->GetPosition(), redGhost->GetEnemyPos());
		redGhost->Update(player->GetDirection(), player->GetPosition(), redGhost->GetEnemyPos());
		pinkGhost->Update(player->GetDirection(), player->GetPosition(), redGhost->GetEnemyPos());
		orangeGhost->Update(player->GetDirection(), player->GetPosition(), redGhost->GetEnemyPos());
		CheckCollisions();
	}
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		blueGhost->DrawPlayer();
		redGhost->DrawPlayer();
		pinkGhost->DrawPlayer();
		orangeGhost->DrawPlayer();
		player->DrawPlayer();
		
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		redGhost->DrawDebug(GREEN);
		pinkGhost->DrawDebug(GREEN);
		blueGhost->DrawDebug(GREEN);
		orangeGhost->DrawDebug(GREEN);
		
	}

	RenderGUI();
	EndMode2D();
}
void Scene::Release()
{
    level->Release();
	player->Release();

	redGhost->Release();
	blueGhost->Release();
	orangeGhost->Release();
	pinkGhost->Release();

	livesUI->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;
	int count = 0;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		count++;
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncreaseScore((*it)->Points());
			if ((*it)->Sounds() == (int)ObjectType::DOT) 
			{
				if (munch1) 
				{
					PlaySound(sound_munch1);
					munch1 = false;
				}
				else 
				{
					PlaySound(sound_munch2);
					munch1 = true;
				}
			}
            else if ((*it)->Sounds() == (int)ObjectType::PELLET) {
				collectPellet = true;
			}
			else if ((*it)->Sounds() == (int)ObjectType::CHERRY or (*it)->Sounds() == (int)ObjectType::STRAWBERRY) {
				PlaySound(sound_fruit);
			}

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}
	if (count == 0) {
		EndLevel = true;
		count = 0;
	}
	else if (count <= FRACTION5_ITEMS) siren = 4;
	else if (count <= FRACTION5_ITEMS * 2) siren = 3;
	else if (count <= FRACTION5_ITEMS * 3) siren = 2;
	else if (count <= FRACTION5_ITEMS * 4) siren = 1;

	if (!god_mode) {
		enemy_box = blueGhost->GetHitbox();
		if (player_box.TestAABB(enemy_box)) {
			if (!blueGhost->IsDead() and !collectPellet) lose = true;
			else {
				if (!BlueCaught) {
					player->IncreaseScore(ghost_points);
					ghost_points *= 2;
				}
				blueGhost->Catched = true;
				BlueCaught = true;
				PlaySound(sound_eatghost);
			}
		}
		else {
			enemy_box = redGhost->GetHitbox();
			if (player_box.TestAABB(enemy_box)) {
				if (!redGhost->IsDead() and !collectPellet) lose = true;
				else {
					if (!RedCaught) {
						player->IncreaseScore(ghost_points);
						ghost_points *= 2;
					}
					redGhost->Catched = true;
					RedCaught = true;
					PlaySound(sound_eatghost);
				}
			}
			else {
				enemy_box = pinkGhost->GetHitbox();
				if (player_box.TestAABB(enemy_box)) {
					if (!pinkGhost->IsDead() and !collectPellet) lose = true;
					else {
						if (!PinkCaught) {
							player->IncreaseScore(ghost_points);
							ghost_points *= 2;
						}
						PinkCaught = true;
						pinkGhost->Catched = true;
						
						PlaySound(sound_eatghost);
					}
				}
				else {
					enemy_box = orangeGhost->GetHitbox();
					if (player_box.TestAABB(enemy_box)) {
						if (!orangeGhost->IsDead() and !collectPellet) lose = true;
						else {
							if (!OrangeCaught) {
								player->IncreaseScore(ghost_points);
								ghost_points *= 2;
							}
							orangeGhost->Catched = true;
							OrangeCaught = true;
							PlaySound(sound_eatghost);
						}
					}
				}
			}
		}
	}
	
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	font->Draw(10, 5, TextFormat("1UP"));
	font->Draw(10, 13, TextFormat("%d", player->GetScore()));
	
	livesUI->RenderUI(player->AddLives());
	livesUI->DrawPlayer();

	if (intro or levelintro) font->Draw((WINDOW_WIDTH / 2)-22, (WINDOW_HEIGHT / 2)+15, TextFormat("READY!"), YELLOW);
	if (intro and intro_count > 60) font->Draw((WINDOW_WIDTH / 2) - 40, (WINDOW_HEIGHT / 2) - 32, TextFormat("PLAYER ONE"), CYANBLUE);

	if(god_mode) font->Draw(WINDOW_WIDTH - 125, 5, TextFormat("GOD MODE ACTIVE"));
}