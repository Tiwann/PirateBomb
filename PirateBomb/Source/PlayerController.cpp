#include "PlayerController.h"
#include "Runtime/Entity.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Math/Vector3.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld2D.h"
#include "Runtime/Application.h"
#include "Runtime/DesktopWindow.h"
#include "Runtime/Scene.h"
#include <print>

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

void PlayerController::OnUpdate(float deltaTime)
{
    Component::OnUpdate(deltaTime);
    Scene* scene = GetScene();
    Ref<PhysicsWorld2D> world = scene->GetPhysicsWorld2D();

    for (const PhysicsBody* body : world->GetBodies())
    {
        const bool awake = body->IsAwake();
        const PhysicsComponent* component = (PhysicsComponent*)body->GetUserPointer();
        const Entity* entity = component->GetOwner();
        std::println(std::cout, "{}: {}", entity->GetObjectName(), awake ? "Awake" : "Sleep" );
    }
}

void PlayerController::OnPhysicsUpdate(const float deltaTime)
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

    if (m_Window->GetKey(KeyCode::KeyW) || m_Window->GetKey(KeyCode::Up)) {
        movementDir += Vector3::Up;
    }
    if (m_Window->GetKey(KeyCode::KeyS) || m_Window->GetKey(KeyCode::Down)) {
        movementDir += Vector3::Down;
    }

    const Vector3 targetVelocity = velocity + movementDir * m_Speed;


    m_PhysicsComponent->SetLinearVelocity(targetVelocity);
}
