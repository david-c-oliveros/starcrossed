#include <string>
#include <map>

#include "world.h"
#include "entity.h"



class Character : public Entity
{
    public:
        Character(glm::vec2 _vWorldPos);
        ~Character();

        void Draw(SpriteRenderer &cRenderer, World &cWorld);
        void AddSpriteSheet(std::string sTexName, std::string sSpriteName);


    public:
        std::map<std::string, Sprite> mSprites;
};
