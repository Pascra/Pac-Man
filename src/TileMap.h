#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	EMPTY = -1,
	// -: check if player has left the screen
	EXIT_LEFT = -3, EXIT_RIGHT = -2,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles (in tileset order)
	a = 1, b, c, d, e, f,
	g, h, i, j, 
	k, l, m, n,
	o, p, 

	q = 17, r, s, t,
	A, B, 
	C, D, 
	E, F,
	G, H,
	I, J, K, L,

	M = 33, O,
	P, Q, R, S,
	T, ab, cd, ef,
	gh, ij, walle, walla, wallo, walli,


	// 50 <= id < 100: special tiles
	DOT = 50, LARGE_DOT, PELLET, FRUIT,

	GHOST_DOOR = 70, GHOST_DOOR02,

	//Intervals
	STATIC_FIRST = a,
	STATIC_LAST = walli,
	SOLID_FIRST = a,
	SOLID_LAST = walli,
	SPECIAL_FIRST = DOT,
	SPECIAL_LAST = PELLET,

	PLAYER = 100,

	//enemies
	Red_Ghost = 101, Pink_Ghost, Blue_Ghost, Orange_Ghost,

	FRUIT_ICON = 200, FRUIT_ICON_1, FRUIT_ICON_2
	

	/*ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER*/
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const;

	bool TestCollisionWallLeft(const AABB& box, bool door) const;
	bool TestCollisionWallRight(const AABB& box, bool door) const;
	bool TestCollisionWallUp(const AABB& box, bool door) const;
	bool TestCollisionWallDown(const AABB& box, bool door) const;

	Tile TestSideExit(const AABB& box) const;
	bool SolidTest(const AABB& box) const;

	bool win = false;
	bool lose = false;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool SolidTile(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	bool CollisionX(const Point& p, int distance, bool door) const;
	bool CollisionY(const Point& p, int distance, bool door) const;

	//Tile map
	Tile* Mapa;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Tile sheet
	const Texture2D* img_tiles;
	const Texture2D* img_tiles_white;

	//count for win screen
	int time = 30;
	int white = 0;
	int flash = 4;
};