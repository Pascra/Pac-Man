#pragma once
#include "Globals.h"
#include "Scene.h"
#include "Intro.h"

enum class GameState { MENU, MAIN_MENU, INTRO, PLAYING, SETTINGS, CREDITS };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus StartPlaying();
    AppStatus BeginIntro();
    void FinishPlay();
    void EndIntro();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    Intro* intro;

    //imagenes
    const Texture2D* menu1;
    const Texture2D* img_menu;
    const Texture2D* vacio;
    const Texture2D* img_menu_rojo;
    const Texture2D* img_menu_rosa;
    const Texture2D* img_menu_azul;
    const Texture2D* img_menu_naranja;
    
    int counter = 0;

    
    RenderTexture2D objetive;
    Rectangle src, dst;
};