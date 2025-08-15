#pragma once
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
};
