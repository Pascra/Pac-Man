#pragma once
#include <raylib.h>
#include "Player.h"
#include "Ghost.h"
#include "TileMap.h"
#include "Object.h"
#include "Text.h"

class Intro 
{
public:
	Intro();
	~Intro();

	AppStatus Init();
	void Update();
	void Render();
	void Release();

	bool loopCheck = false;

private:
	AppStatus LoadIntro();
	void CheckCollisions();

	int timer = 0;

	bool turn = false;
	bool loop = false;
	bool end = true;
	

	bool isorangeGhost = false;
	bool isredGhost = false;
	bool isblueGhost = false;
	bool ispinkGhost = false;
	bool isDot = true;

	Player* PacMan;

	Ghost* Red;
	Ghost* Pink;
	Ghost* Blue;
	Ghost* Orange;

	Object* Dot;

	Camera2D camera;
	TileMap* introScene;
	int pacmanX, pacmanY;
	int dotX, dotY;
};