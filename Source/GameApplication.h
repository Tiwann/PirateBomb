#pragma once
#include "Runtime/GameApplication.h"

namespace Nova::Rendering { class Texture; }

class GameApplication : public Nova::Application
{
public:

    void OnInit() override;
    void OnUpdate(float deltaTime) override;

    Nova::Rendering::Texture* LoadTexture(const Nova::String& name, Nova::StringView filepath);
};
