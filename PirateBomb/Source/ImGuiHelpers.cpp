#include "ImGuiHelpers.h"
#include "Components/Transform.h"
#include <imgui.h>

#include "Components/Physics/BoxComponent2D.h"
#include "Components/Rendering/SpriteRenderer.h"

void DrawTransform(const Nova::StringView name, Nova::Transform* transform)
{
    if (!transform) return;
    ImGui::PushID(transform);
    if (ImGui::TreeNode(*name))
    {
        Nova::Vector3 position = transform->GetPosition();
        if ((ImGui::DragFloat3("Position", position.ValuePtr(), 0.01f, 0, 0, "%.2f")))
            transform->SetPosition(position);

        Nova::Vector3 eulerAngles = transform->GetRotation().ToEulerDegrees();
        if ((ImGui::DragFloat3("Rotation", eulerAngles.ValuePtr(), 0.01f, 0, 0, "%.2f")))
            transform->SetRotation(Nova::Quaternion::FromEulerDegrees(eulerAngles));

        Nova::Vector3 scale = transform->GetScale();
        if ((ImGui::DragFloat3("Scale", scale.ValuePtr(), 0.01f, 0, 0, "%.2f")))
            transform->SetScale(scale);

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void DrawFps(const float deltaTime, const float rounding)
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);
    if (ImGui::Begin("FPS counter", nullptr, flags))
    {
        static char fpsBuff[9] = "0000 fps";
        static float timer = 0.0f;
        timer += deltaTime;
        if (timer > 0.2f)
        {
            const uint32_t fps = 1.0f / deltaTime;
            std::format_to(fpsBuff, "{:04} fps", fps);
            timer = 0.0f;
        }

        ImGui::TextUnformatted(fpsBuff, fpsBuff + 9);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void DrawSpriteRenderer(const Nova::StringView name, Nova::SpriteRenderer* renderer)
{
    if (!renderer) return;
    ImGui::PushID(renderer);
    if (ImGui::TreeNode(*name))
    {
        Nova::Vector2 tiling = renderer->GetTiling();
        if (ImGui::DragFloat2("Tiling", tiling.ValuePtr(), 0.01f, 0, 0, "%.2f"))
            renderer->SetTiling(tiling);

        int32_t fps = 1.0f / renderer->GetSpeed();
        if (ImGui::SliderInt("Speed", &fps, 0, 120))
            renderer->SetSpeed(fps);

        const Nova::StringView flagNames[] = { "Tile With Scale", "Flip Horizontal", "Flip Vertical" };

        if(ImGui::BeginCombo("Flags", nullptr, ImGuiComboFlags_NoPreview))
        {
            Nova::SpriteRendererFlags flags = renderer->GetFlags();
            for(size_t i = 0; i < std::size(flagNames); i++)
            {
                bool selected = flags.Contains(Nova::SpriteRendererFlagBits(1 << i));
                if(ImGui::Selectable(flagNames[i], &selected))
                {
                    flags.Toggle(Nova::SpriteRendererFlagBits(1 << i));
                    renderer->SetFlags(flags);
                }
            }
            ImGui::EndCombo();
        }

        Nova::Color colorTint = renderer->GetColorTint();
        if (ImGui::ColorEdit4("Color Tint", (float*)&colorTint, ImGuiColorEditFlags_DisplayHex))
            renderer->SetColorTint(colorTint);

        uint32_t pixelsPerUnit = renderer->GetPixelsPerUnit();
        if (ImGui::DragInt("Pixels Per Unit", (int*)&pixelsPerUnit, 1, 0, 0))
            renderer->SetPixelsPerUnit(pixelsPerUnit);

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void DrawBoxComponent(Nova::StringView name, Nova::BoxComponent2D* box)
{
    if (!box) return;
    ImGui::PushID(box);
    if (ImGui::TreeNode(*name))
    {
        Nova::Vector2 position = box->GetShapePosition();
        if (ImGui::DragFloat2("Position", position.ValuePtr(), 0.01f, 0, 0, "%.2f"))
            box->SetShapePosition(position);

        Nova::Vector2 size = Nova::Vector2(box->GetWidth(), box->GetHeight());
        if (ImGui::DragFloat2("Size", size.ValuePtr(), 0.01f, 0, 0, "%.2f"))
            box->SetSize(size.x, size.y);

        float rotation = Nova::Math::Degrees(box->GetShapeRotation());
        if (ImGui::DragFloat("Rotation", &rotation, 0.01f, 0, 0, "%.2f"))
            box->SetShapeRotation(Nova::Math::Radians(rotation));

        ImGui::TreePop();
    }
    ImGui::PopID();
};
