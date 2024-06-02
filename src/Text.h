#pragma once
#include "Globals.h"
#include <string>
#include "raylib.h"
#include "ResourceManager.h"

class Text
{
public:
	Text();
	~Text();
	AppStatus Initialise(Resource id, const std::string& file_path, char first_character, int character_size);

	
	void Draw(int x, int y, const std::string& text, const Color& col = WHITE) const;

private:
	// Method to draw a single character at a specified position
	void DrawCharacter(int x, int y, char c, const Color& col = WHITE) const;

	char first_character;	
	int character_size;		//mida personage
			
	int rows;				//cantidad de row
	int columns;	//cantidad de columnas

	const Texture2D* img;	//reference to the texture containing the characters
};