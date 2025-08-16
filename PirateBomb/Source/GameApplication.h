#pragma once
#include "GameAssets.h"
#include "Runtime/Application.h"

namespace Nova::Rendering { class Texture; }
namespace Nova { class SpriteAnimation; struct SpriteSheet; }

class GameApplication : public Nova::Application
{
public:
    Nova::ApplicationConfiguration GetConfiguration() const override;
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnGUI() override;

    Nova::Ref<Nova::Rendering::Texture> LoadTexture(const Nova::String& name, Nova::StringView filepath);
    Nova::Ref<Nova::SpriteAnimation> LoadSpriteAnimation(const Nova::String& name, const Nova::SpriteSheet& spriteSheet);
    void LoadTexture(GameAssets asset, Nova::StringView filepath);
    void LoadAnimation(GameAssets texture, GameAssets animation, uint32_t rows, uint32_t columns, uint32_t spriteCount, uint32_t spriteSize);
private:
    bool m_EnableGui = false;
};
