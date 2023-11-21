#include "tile.h"



Tile::Tile(const char* _sTexFilepath, glm::ivec2 _vCoords, glm::vec2 _vSize)
    : vCoords(_vCoords), vSize(_vSize)
{
    cSprite = std::make_unique<Sprite>(_sTexFilepath, true, vSize, DEFAULT_TILE_COLOR, DEFAULT_TILE_ROT);
    std::cout << "Position in grid: " << glm::to_string(vCoords) << std::endl;
}



void Tile::Create()
{
}



Tile::~Tile()
{
}



void Tile::Draw(SpriteRenderer &cRenderer)
{
    cSprite->Draw(cRenderer, vCoords);
}
