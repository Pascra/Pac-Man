#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
    //iniciar audio
    InitAudioDevice();
    LoadSounds();
}
ResourceManager::~ResourceManager()
{
    Release();
}

//Load a texture from a file and associate it with the key id
AppStatus ResourceManager::LoadTexture(Resource id, const std::string& file_path)
{
    //Load the texture
    Texture2D texture = ::LoadTexture(file_path.c_str());
    if (texture.id == 0)
    {
        //Error loading texture
        LOG("Failed to load texture ", file_path);
        return AppStatus::ERROR;
    }

    //Insert the loaded texture into the map with the specified key
    textures[id] = texture;
    return AppStatus::OK;
}

//cargar todos los sonidos
void ResourceManager::LoadSounds()
{
    sounds[(int)AudioResource::INtro] = LoadSound("resource/audios/game_start.wav");
    sounds[(int)AudioResource::munch_1] = LoadSound("resource/audios/munch_1.wav");
    sounds[(int)AudioResource::munch_2] = LoadSound("resource/audios/munch_2.wav");
    sounds[(int)AudioResource::PowerPellet] = LoadSound("resource/audios/power_pellet.wav");
    sounds[(int)AudioResource::eatFruit] = LoadSound("resource/audios/eat_fruit.wav");
     sounds[(int)AudioResource::SIREN1] = LoadSound("resource/audios/siren_1.wav");
    sounds[(int)AudioResource::SIREN2] = LoadSound("resource/audios/siren_2.wav");
    sounds[(int)AudioResource::SIREN3] = LoadSound("resource/audios/siren_3.wav");
    sounds[(int)AudioResource::SIREN4] = LoadSound("resource/audios/siren_4.wav");
    sounds[(int)AudioResource::SIREN5] = LoadSound("resource/audios/siren_5.wav");
    sounds[(int)AudioResource::KillGhost] = LoadSound("resource/audios/eat_ghost.wav");
    sounds[(int)AudioResource::retreating] = LoadSound("resource/audios/retreating.wav");
    sounds[(int)AudioResource::pacmanDeath] = LoadSound("resource/audios/pacman_death.wav");
}

//Release the texture associated with the key id
void ResourceManager::ReleaseTexture(Resource id)
{
    //Find the texture associated with the key
    auto it = textures.find(id);

    //If found, unload the texture and remove it from the map
    if (it != textures.end())
    {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

//Get a texture by key
const Texture2D* ResourceManager::GetTexture(Resource id) const
{
    //Find the texture associated with the key and return it
    auto it = textures.find(id);
    if (it != textures.end())   return &(it->second);

    //Return nullptr if key is not found
    return nullptr;
}

Sound ResourceManager::GetSound(AudioResource id) const
{
    return (sounds[(int)id]);
}

void ResourceManager::Release()
{
    for (auto& pair : textures)
    {
        UnloadTexture(pair.second);
    }
    textures.clear();
    for (int i = 0; i < (int)AudioResource::AUD_NUM; ++i) {
        UnloadSound(sounds[i]);
    }
    CloseAudioDevice();
}