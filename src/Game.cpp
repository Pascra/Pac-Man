#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::MENU;
    scene = nullptr;

    intro = nullptr;

    img_menu = nullptr;
    vacio = nullptr;
    img_menu_rojo = nullptr;
    img_menu_rosa = nullptr;
    img_menu_azul = nullptr;
    img_menu_naranja = nullptr;

    menu1 = nullptr;

    objetive = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    //escena
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
    //intro
    if (intro != nullptr)
    {
        intro->Release();
        delete intro;
        intro = nullptr;
    }
}
AppStatus Game::Initialise(float mesure)
{
    float w, h, w2, h2;
    w = WINDOW_WIDTH * mesure;
    h = WINDOW_HEIGHT * mesure;

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man");

    int Screen = GetCurrentMonitor();
    w2 = (float)GetMonitorWidth(Screen);
    h2 = (float)GetMonitorHeight(Screen);

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    objetive = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (objetive.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(objetive.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { w2/2 - w/2, h2/2 - h/2, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    SetWindowSize(w2, h2);
    ToggleFullscreen();

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "resource/Sprites/paginaPrincipal.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::vacia, "resource/Sprites/paginavacia.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    vacio = data.GetTexture(Resource::vacia);

    if (data.LoadTexture(Resource::IMG_MENU_naranja, "resource/Sprites/naranja.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_naranja = data.GetTexture(Resource::IMG_MENU_naranja);

    if (data.LoadTexture(Resource::IMG_MENU_rojo, "resource/Sprites/rojo.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_rojo = data.GetTexture(Resource::IMG_MENU_rojo);

    if (data.LoadTexture(Resource::IMG_MENU_rosa, "resource/Sprites/rosa.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_rosa = data.GetTexture(Resource::IMG_MENU_rosa);

    if (data.LoadTexture(Resource::IMG_MENU_azul, "resource/Sprites/azul.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_azul = data.GetTexture(Resource::IMG_MENU_azul);


    //----------------------------------------------------------
    //portada de grupo (frame studios)
    if (data.LoadTexture(Resource::Imagen_Menu, "resource/Sprites/Portada.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    menu1 = data.GetTexture(Resource::Imagen_Menu);

    return AppStatus::OK;
}
AppStatus Game::StartPlaying()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::BeginIntro()
{
    intro = new Intro();
    if (intro == nullptr)
    {
        LOG("Failed to allocate memory for intro");
        return AppStatus::ERROR;
    }
    if (intro->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise intro");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::EndIntro()
{
    intro->Release();
    delete intro;
    intro = nullptr;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}

AppStatus Game::Update()
{
    //comprueba manera de cerrar juego
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {

    case GameState::MENU:
     if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
     if(IsKeyPressed(KEY_SPACE))
     {
         if(BeginIntro() != AppStatus::OK) return AppStatus::ERROR;
         state = GameState::MAIN_MENU;
     }
     break;

    case GameState::MAIN_MENU:


        if (IsKeyPressed(KEY_ESCAPE))
        {
            EndIntro();
            counter = 0;
            state = GameState::MENU;
        }
        else if (IsKeyPressed(KEY_SPACE))
        {
            EndIntro();
            counter = 0;
            if (StartPlaying() != AppStatus::OK) return AppStatus::ERROR;
            scene->intro = true;
            state = GameState::PLAYING;
        }
        else {
            if (counter >= 840) {
                intro->Update();
                if (intro->loopCheck) counter = 0;
            }
            counter++;
        }
        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE) || scene->EndGame)
        {
            FinishPlay();
            if (BeginIntro() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::MAIN_MENU;
        }
        else
        {
            
            scene->Update();
        }
        break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //dibujar
    BeginTextureMode(objetive);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::MENU:
          
        DrawTexture(*menu1, 0, 0, WHITE);
        break;

    case GameState::MAIN_MENU:
        if (counter <= 60) {
            DrawTexture(*vacio, 0, 0, WHITE);
        }
        else if (counter <= 240) {
            DrawTexture(*img_menu_rojo, 0, 0, WHITE);
            if (counter <= 120) DrawRectangle(7 * TILE_SIZE, 7 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 180) DrawRectangle(18 * TILE_SIZE, 7 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 600) {
            DrawTexture(*img_menu_azul, 0, 0, WHITE);
            if (counter <= 480) DrawRectangle(7 * TILE_SIZE, 13 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 540) DrawRectangle(18 * TILE_SIZE, 13 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 420) {
            DrawTexture(*img_menu_rosa, 0, 0, WHITE);
            if (counter <= 300) DrawRectangle(7 * TILE_SIZE, 9 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 360) DrawRectangle(18 * TILE_SIZE, 9 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else {
            DrawTexture(*img_menu, 0, 0, WHITE);
            if (counter == 840) intro->loopCheck = false;
            if (counter >= 840) {
                intro->Render();
            }

            else if (counter <= 780) {
                DrawTexture(*img_menu_naranja, 0, 0, WHITE);
                if (counter <= 660) DrawRectangle(7 * TILE_SIZE, 15 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
                else if (counter <= 720) DrawRectangle(18 * TILE_SIZE, 15 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            }
        }
        break;

    case GameState::PLAYING:
        scene->Render();
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(objetive.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_MENU_rojo);
    data.ReleaseTexture(Resource::IMG_MENU_rosa);
    data.ReleaseTexture(Resource::IMG_MENU_azul);
    data.ReleaseTexture(Resource::IMG_MENU_naranja);
    data.ReleaseTexture(Resource::vacia);
    data.ReleaseTexture(Resource::Imagen_Menu);

    UnloadRenderTexture(objetive);
}