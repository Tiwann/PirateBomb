#pragma once
#include "Components/Physics/PlaneComponent2D.h"
#include "Containers/StringView.h"

namespace Nova
{
    class Transform;
    class SpriteRenderer;
    class BoxComponent2D;
};

void DrawTransform(Nova::StringView name, Nova::Transform* transform);
void DrawFps(float deltaTime, float rounding);
void DrawSpriteRenderer(Nova::StringView name, Nova::SpriteRenderer* renderer);
void DrawBoxComponent2D(Nova::StringView namer, Nova::BoxComponent2D* box);
void DrawPlaneComponent2D(Nova::StringView name, Nova::PlaneComponent2D* plane);