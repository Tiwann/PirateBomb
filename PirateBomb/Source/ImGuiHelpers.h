#pragma once
#include "Containers/StringView.h"

namespace Nova
{
    class Transform;
    class SpriteRenderer;
};

void DrawTransform(Nova::StringView name, Nova::Transform* transform);
void DrawFps(float deltaTime, float rounding);
void DrawSpriteRenderer(Nova::StringView name, Nova::SpriteRenderer* renderer);