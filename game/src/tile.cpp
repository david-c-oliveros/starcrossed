#include "tile.h"



Tile::Tile(const char* _sTexFilepath, glm::ivec2 _vCoords, glm::vec2 _vSizeScalar)
    : vCoords(_vCoords), vSizeScalar(_vSizeScalar)
{
    // TODO - Move handling of exact coordinates into engine code
    cSprite = std::make_unique<Sprite>(_sTexFilepath, true, vSizeScalar, DEFAULT_TILE_COLOR, DEFAULT_TILE_ROTATION);
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
