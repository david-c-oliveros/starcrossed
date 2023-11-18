#include "tile.h"



Tile::Tile(glm::ivec2 _vCoords, const char* _sTexFilepath)
    : vCoords(_vCoords)
{
    cSprite = std::make_unique<Sprite>(_sTexFilepath, true, DEFAULT_TILE_SIZE, DEFAULT_TILE_COLOR, DEFAULT_TILE_ROT);
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
