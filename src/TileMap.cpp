#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	Mapa = nullptr;
	width = 0;
	height = 0;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (Mapa != nullptr)
	{
		delete[] Mapa;
		Mapa = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;
	const int k = TILE_SIZE + 1; //for the gray border in the tileset

	dict_rect[(int)Tile::AIR] = { 12*k,  2*k, n, n };

	dict_rect[(int)Tile::a] = { 0,  0, n, n };
	dict_rect[(int)Tile::b] = { k,  0, n, n };
	dict_rect[(int)Tile::c] = { 2*k, 0, n, n };
	dict_rect[(int)Tile::d] = { 3 * k, 0, n, n };
	dict_rect[(int)Tile::e] = { 4 * k, 0, n, n };
	dict_rect[(int)Tile::f] = { 5 * k, 0, n, n };
	dict_rect[(int)Tile::g] = { 6 * k, 0, n, n };
	dict_rect[(int)Tile::h] = { 7 * k, 0, n, n };
	dict_rect[(int)Tile::i] = { 8 * k, 0, n, n };
	dict_rect[(int)Tile::j] = { 9 * k, 0, n, n };
	dict_rect[(int)Tile::k] = { 10 * k, 0, n, n };
	dict_rect[(int)Tile::l] = { 11 * k, 0, n, n };
	dict_rect[(int)Tile::m] = { 12 * k, 0, n, n };
	dict_rect[(int)Tile::n] = { 13 * k, 0, n, n };
	dict_rect[(int)Tile::o] = { 14 * k, 0, n, n };
	dict_rect[(int)Tile::p] = { 15 * k, 0, n, n };

	dict_rect[(int)Tile::q] = { 0,  k, n, n };
	dict_rect[(int)Tile::r] = { k,  k, n, n };
	dict_rect[(int)Tile::s] = { 2 * k, k, n, n };
	dict_rect[(int)Tile::t] = { 3 * k, k, n, n };
	dict_rect[(int)Tile::A] = { 4 * k, k, n, n };
	dict_rect[(int)Tile::B] = { 5 * k, k, n, n };
	dict_rect[(int)Tile::C] = { 6 * k, k, n, n };
	dict_rect[(int)Tile::D] = { 7 * k, k, n, n };
	dict_rect[(int)Tile::E] = { 8 * k, k, n, n };
	dict_rect[(int)Tile::F] = { 9 * k, k, n, n };
	dict_rect[(int)Tile::G] = { 10 * k, k, n, n };
	dict_rect[(int)Tile::H] = { 11 * k, k, n, n };
	dict_rect[(int)Tile::I] = { 12 * k, k, n, n };
	dict_rect[(int)Tile::J] = { 13 * k, k, n, n };
	dict_rect[(int)Tile::K] = { 14 * k, k, n, n };
	dict_rect[(int)Tile::L] = { 15 * k, k, n, n };

	dict_rect[(int)Tile::M] = { 0,  2*k, n, n };
	dict_rect[(int)Tile::O] = { k,  2 * k, n, n };
	dict_rect[(int)Tile::P] = { 2 * k, 2 * k, n, n };
	dict_rect[(int)Tile::Q] = { 3 * k, 2 * k, n, n };
	dict_rect[(int)Tile::R] = { 4 * k, 2 * k, n, n };
	dict_rect[(int)Tile::S] = { 5 * k, 2 * k, n, n };
	dict_rect[(int)Tile::T] = { 6 * k, 2 * k, n, n };
	dict_rect[(int)Tile::ab] = { 7 * k, 2 * k, n, n };
	dict_rect[(int)Tile::cd] = { 8 * k, 2 * k, n, n };
	dict_rect[(int)Tile::ef] = { 9 * k, 2 * k, n, n };
	dict_rect[(int)Tile::gh] = { 10 * k, 2 * k, n, n };
	dict_rect[(int)Tile::ij] = { 11 * k, 2 * k, n, n };
	dict_rect[(int)Tile::walle] = { 4 * k, 3 * k, n, n };
	dict_rect[(int)Tile::walla] = { 5 * k, 3 * k, n, n };
	dict_rect[(int)Tile::wallo] = { 7 * k, 3 * k, n, n };
	dict_rect[(int)Tile::walli] = { 6 * k, 3 * k, n, n };

	dict_rect[(int)Tile::FRUIT_ICON_1] = { 0, 3 * k, n*2, n*2 };
	dict_rect[(int)Tile::FRUIT_ICON_2] = { n*2, 3 * k, n*2, n*2 };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_Tiles, "resource/Sprites/Arcade - Pac-Man - Maze Parts-only_blue_tiles2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_Tiles);

	if (data.LoadTexture(Resource::whiteTiles, "resource/Sprites/Arcade - Pac-Man - Maze Parts-only_white_tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles_white = data.GetTexture(Resource::whiteTiles);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (Mapa != nullptr)	delete[] Mapa;

	Mapa = new Tile[size];
	if (Mapa == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(Mapa, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	int y = 0;
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return Mapa[x + y * width];
}
bool TileMap::SolidTile(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
//colisiones
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	return CollisionY(box.pos, box.height);
}
bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	return CollisionY(box.pos + Point(0, box.height - 1), box.height);
}
bool TileMap::TestCollisionWallLeft(const AABB& box, bool door) const
{
	if (!door) return CollisionX(box.pos, box.height, door);
	else return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box, bool door) const
{
	if (!door) return CollisionX(box.pos + Point(box.width - 1, 0), box.height, door);
	else CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box, bool door) const
{
	if (!door) return CollisionY(box.pos, box.height, door);
	else CollisionY(box.pos, box.height);
}
bool TileMap::TestCollisionWallDown(const AABB& box, bool door) const
{
	if (!door) return CollisionY(box.pos + Point(0, box.height - 1), box.height, door);
	else CollisionY(box.pos + Point(0, box.height - 1), box.height);
}
Tile TileMap::TestSideExit(const AABB& box) const
{
	return GetTileIndex(box.pos.x, box.pos.y);
}
bool TileMap::SolidTest(const AABB& box) const
{
	return SolidTile(GetTileIndex(box.pos.x, box.pos.y));
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (SolidTile(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (SolidTile(tile))
			return true;
	}
	return false;
}
bool TileMap::CollisionX(const Point& p, int distance, bool door) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (SolidTile(GetTileIndex(x, y))) return true;
		if (GetTileIndex(x, y) == Tile::GHOST_DOOR or GetTileIndex(x, y) == Tile::GHOST_DOOR02) return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance, bool door) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (SolidTile(tile)) return true;
		if (tile == Tile::GHOST_DOOR or tile == Tile::GHOST_DOOR02) return true;
	}
	return false;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle RecTan;
	Vector2 position;

	if (win) {
		if (white == 0) {
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					tile = Mapa[i * width + j];
					if (tile != Tile::AIR)
					{
						position.x = (float)j * TILE_SIZE;
						position.y = (float)i * TILE_SIZE;
						RecTan = dict_rect[(int)tile];
						DrawTextureRec(*img_tiles_white, RecTan, position, WHITE);

					}
				}
			}
			--time;
			if (time <= 0) {
				time = 30;
				white = 4;
				--flash;
			}
		}
		else {
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					tile = Mapa[i * width + j];
					if (tile != Tile::AIR)
					{
						position.x = (float)j * TILE_SIZE;
						position.y = (float)i * TILE_SIZE;
						RecTan = dict_rect[(int)tile];
						DrawTextureRec(*img_tiles, RecTan, position, WHITE);

					}
				}
			}
			--time;
			if (time <= 0) {
				time = 30;
				white = 0;
				--flash;
			}
		}
		if (flash == 0) {
			win = false;
			flash = 4;
		}

	}
	else {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				tile = Mapa[i * width + j];
				if (tile != Tile::AIR)
				{
					position.x = (float)j * TILE_SIZE;
					position.y = (float)i * TILE_SIZE;
					RecTan = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, RecTan, position, WHITE);

				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_Tiles);
	data.ReleaseTexture(Resource::whiteTiles);

	dict_rect.clear();
}