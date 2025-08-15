#include "PlayerController.h"
#include "Runtime/Entity.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Math/Vector3.h"
#include "Runtime/Application.h"
#include "Runtime/DesktopWindow.h"

using namespace Nova;

void PlayerController::OnInit()
{
    const Entity* owner = GetOwner();
    const Application* application = GetApplication();
    m_PhysicsComponent = owner->GetComponent<PhysicsComponent>();
    if (!m_PhysicsComponent)
        SetEnabled(false);

    m_Window = application->GetWindow();
}

void PlayerController::OnUpdate(const float deltaTime)
{
    const Vector3 lastVelocity = m_PhysicsComponent->GetLinearVelocity();
    const Vector3 velocity = Vector3::Zero.WithY(lastVelocity.y);

    Vector3 movementDir = Vector3::Zero;
    if (m_Window->GetKey(KeyCode::KeyA) || m_Window->GetKey(KeyCode::Left)) {
        movementDir += Vector3::Left;
    }
    if (m_Window->GetKey(KeyCode::KeyD) || m_Window->GetKey(KeyCode::Right)) {
        movementDir += Vector3::Right;
    }

    const Vector3 targetVelocity = velocity + movementDir * m_Speed;
    //const Vector3 smoothedVelocity = Vector3::Lerp(lastVelocity, targetVelocity, 1.0f - Math::Exp(-m_Acceleration * deltaTime));
    m_PhysicsComponent->SetLinearVelocity(targetVelocity);
}
