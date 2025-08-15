#pragma once
#include "Runtime/Component.h"
#include "Runtime/Ref.h"

namespace Nova
{
    class PhysicsComponent;
    class Window;
    class DesktopWindow;
}

class PlayerController final : public Nova::Component
{
public:
    PlayerController(Nova::Entity* owner) : Component(owner, "Player Component"){}

    void OnInit() override;
    void OnUpdate(float deltaTime) override;

private:
    Nova::Ref<Nova::DesktopWindow> m_Window = nullptr;
    Nova::PhysicsComponent* m_PhysicsComponent = nullptr;
    float m_Speed = 1.0f;
    float m_Acceleration = 5.0f;
};
