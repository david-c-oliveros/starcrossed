#include <string>
#include <map>

#include "world.h"
#include "entity.h"
#include "animated_sprite.h"



class Character : public Entity
{
    public:
        Character(glm::vec2 _vWorldPos);
        ~Character();

        void Draw(SpriteRenderer &cRenderer, World &cWorld);
        void AddAnimatedSprite(std::string sTexName, std::string sSpriteName);
        void ConfigAnimatedSprite(std::string sSpriteName, uint32_t nNumFrames, uint32_t nTicksPerFrame,
                                  glm::vec2 vStart, glm::vec2 vFrameSize, glm::vec2 vFrameOffset,
                                  glm::vec2 vSpriteScalar = glm::vec2(1.0f));
        void SetCurrentSprite(std::string sSpriteName);
        void StartSpriteAnim();
        void Update();


    public:
        std::map<std::string, AnimatedSprite> mSprites;

        std::vector<std::string> vecDebugInfo;

    private:
        std::string m_sCurSprite;
};
