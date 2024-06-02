#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

//Imagenes
enum class Resource {
    IMG_MENU_rojo,
    IMG_MENU_azul,
    IMG_MENU_rosa,
    IMG_MENU_naranja,
    Imagen_Menu,
    IMG_MENU,
    vacia,
    whiteTiles,
    IMG_Ghost,
    IMG_ITEMS,
    IMG_FONT,
    IMG_PLAYER,
    IMG_Tiles
};
//sonidos
enum class AudioResource {
    INtro,
    munch_1,
    munch_2,
    pacmanDeath,
    SIREN5,
    PowerPellet,
    eatFruit,
    KillGhost,
    retreating,
    SIREN1,
    SIREN2,
    SIREN3,
    SIREN4,
    AUD_NUM
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void LoadSounds();

    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;
    Sound GetSound(AudioResource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
    Sound sounds[(int)AudioResource::AUD_NUM];
};