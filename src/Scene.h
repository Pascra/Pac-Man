#pragma once
#include <raylib.h>
#include "Player.h"
#include "Ghost.h"
#include "TileMap.h"
#include "Object.h"
#include "UI.h"
#include "Text.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

//total amount of created levels
#define LEVELS 2

//amount of items in a level divided by 5 (for the siren)
#define FRACTION5_ITEMS 48

//siren length
#define SIREN_LENGTH 48

//font color
#define CYANBLUE CLITERAL(Color){0, 255, 255, 255}

//timers
#define PELLETTemps 480;
#define FRUITTIME 900;

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

    bool intro = false;

    bool EndGame = false;
    
    bool levelintro = 0;

private:
    AppStatus LoadLevel(int stage);

    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    void CheckCollisions();
    void ClearLevel();
   

    void RenderGUI() const;

    Player* player;

    //fantasmas
    Ghost* redGhost;
    Ghost* blueGhost;
    Ghost* orangeGhost;
    Ghost* pinkGhost;

    //UI
    UI* livesUI;

    TileMap* level;
    std::vector<Object*> objects;

    //sorolls
    int siren_length = SIREN_LENGTH;

    int siren = 0;

    bool collectPellet = false;
    int contador_pellet = 480;
    Sound sound_intro, sound_munch1, sound_munch2, sound_pellet, sound_fruit, sound_eatghost;
   
    Sound sirens[5];
    
    

    //joc
    bool EndLevel = false;
    bool win = false;
    bool lose = false;
    int level_count = 1;

    
    bool munch1 = true;

    //comprobar el godmode
    bool god_mode = false;

   
    int intro_count = 240;

    //pocicions (player, ghosts i fruits)
    int fruitX, fruitY;
    int fruitcounter = FRUITTIME;
    int pacmanX, pacmanY;
    int RedX, RedY;
    int PinkX, PinkY;
    int AzulX, AzulY;
    int OrangeX, OrangeY;
   

    bool RedCaught = false;
    bool BlueCaught = false;
    bool PinkCaught = false;
    bool OrangeCaught = false;

    int ghost_points = 200;

    Text* font;

    Camera2D camera;
    DebugMode debug;
};