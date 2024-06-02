#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 8x8
#define ENEMY_FRAME_SIZE		8

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	8
#define ENEMY_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define Ghost_SPEED			1

//Logic states
enum class State2 { Freeze, Moving, PELLET, EYES };
enum class Look2 { RIGHT, LEFT, UP, DOWN };
enum class Mode2 { Hunt, Scared };
enum class GhostType { red, pink, blue, orange };

//Rendering states
enum class GhostAnim {
	FREEZE,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, PELLET, PELLET_FLASH,
	 HIDDEN, EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	NUM_ANIMATIONS
};

class Ghost : public Entity
{
public:
	Ghost(const Point& p, State2 s, Look2 view, GhostType t);
	~Ghost();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	bool Catched = false;
	bool Puerta = false;
	void Intro(int count);
	void Pellet(bool ifPellet, int count);
	void SetObjective(Point t);
	void CancelObjective();
	void SetHome(Point t);
	void SetHomeExit(Point t);
	bool IsDead();
	
	Point GetEnemyPos();

	void Update(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void Release();
	void DrawDebug(const Color& col) const;
	void WinLose();

	bool introCatch = false;
	void IntroUpdate(bool turn);

private:
	//direcciones
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//acciones enemigas
	float GetTargetDistance(Point dir);
	void Move(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void UpdateTarget(Point pacmanDir, Point pacmanPos, Point blinkypos);

	//Animation management
	void SetAnimation(int id);
	GhostAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void StartDying();
	void SwitchAnimationRight();
	void SwitchAnimationLeft();
	void SwitchAnimationUp();
	void SwitchAnimationDown();

	void UpdateView(int anim_id);
	int current_step;
	int current_frames;

	State2 state;
	Look2 look;
	Mode2 mode;
	GhostType type;
	TileMap* mapa;

	Sound sound_retreat;

	Point home;
	Point Leave_Home;
	Point Objective;

	bool vulnearble = false;
	bool flash = true;
	int delay = 30;

};
