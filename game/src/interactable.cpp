#include "interactable.h"



Interactable::Interactable(glm::vec2 _vPos, glm::vec2 _vSize)
    : vPos(_vPos), vSize(_vSize)
{
    vecDebugInfo.push_back("Interactable");
    for (int32_t i = 1; i < 8; i++)
        vecDebugInfo.push_back("");

    cSprite = std::make_unique<Sprite>("spaceship_sheet");
    cSpriteSolid = std::make_unique<Sprite>();
    cSpriteSolid->SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
}



Interactable::~Interactable()
{
}



void Interactable::Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld)
{
    glm::vec2 vScreenPos = cTileWorld.WorldToScreen(vPos);
    cSpriteSolid->DrawColored(cRenderer, vScreenPos, cTileWorld.GetWorldScale());

    vecDebugInfo[3] = "Rendering at screen position: " + glm::to_string(vScreenPos);
}



bool Interactable::IsHovered(TileWorld &cTileWorld, glm::vec2 vCursorScreenPos)
{
    glm::vec2 vMin = cTileWorld.WorldToScreen(vPos);
    glm::vec2 vMax = (glm::vec2)cTileWorld.WorldToScreen(vPos) + vSize * cTileWorld.GetWorldScale();

    vecDebugInfo[1] = "vMin: " + glm::to_string(vMin);
    vecDebugInfo[2] = "vMax: " + glm::to_string(vMax);

    return vCursorScreenPos.x >  vMin.x && vCursorScreenPos.y >  vMin.y &&
           vCursorScreenPos.x <= vMax.x && vCursorScreenPos.y <= vMax.y;
}
