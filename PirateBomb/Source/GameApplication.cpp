#include "GameApplication.h"
#include "Rendering/Texture.h"
#include "Rendering/Vulkan/Texture.h"
#include "Runtime/Path.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/Rendering/SpriteRenderer.h"
#include "Runtime/SpriteAnimation.h"
#include "Runtime/DesktopWindow.h"
#include "Runtime/Scene.h"
#include "ImGuiHelpers.h"
#include "Runtime/EntryPoint.h"
#include "Components/Physics/BoxComponent2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"

#include <cstdint>
#include <imgui.h>

#include "Components/Physics/PlaneComponent2D.h"


NOVA_DEFINE_APPLICATION_CLASS(GameApplication)

using namespace Nova;
using namespace Nova::Rendering;

static Transform* transform = nullptr;
static SpriteRenderer* renderer = nullptr;
static BoxComponent2D* box = nullptr;
static SpriteAnimation* playerIdleAnim = nullptr;
static SpriteAnimation* playerRunAnim = nullptr;

ApplicationConfiguration GameApplication::GetConfiguration() const
{
    ApplicationConfiguration config;
    config.windowWidth = 800;
    config.windowHeight = 600;
    config.vsync = false;
    config.applicationName = "Pirate Bomb";
    config.fullscreen = false;
    config.resizable = false;
    return config;
}

void GameApplication::OnInit()
{
    const Ref<Texture> playerIdleTexture = LoadTexture("Tex-PlayerIdle", Path::GetAssetPath("Sprites/Player-Idle-58.png"));
    const Ref<Texture> playerRunTexture = LoadTexture("Tex-PlayerRun", Path::GetAssetPath("Sprites/Player-Run-58.png"));
    const Ref<Texture> playerJumpTexture = LoadTexture("Tex-PlayerJump", Path::GetAssetPath("Sprites/Player-Jump-58.png"));
    const Ref<Texture> playerHitTexture = LoadTexture("Tex-PlayerHit", Path::GetAssetPath("Sprites/Player-Hit-58.png"));
    const Ref<Texture> playerGroundTexture = LoadTexture("Tex-PlayerGround", Path::GetAssetPath("Sprites/Player-Ground-58.png"));

    playerIdleAnim = LoadSpriteAnimation("Anm-PlayerIdle", SpriteSheet(2, 13, 26, 58, playerIdleTexture));
    playerRunAnim = LoadSpriteAnimation("Anm-PlayerRun", SpriteSheet(2, 7, 14, 58, playerRunTexture));

    const auto constructScene = [this] -> Scene* {
        const RenderTarget* renderTarget = GetRenderTarget();

        Scene* scene = new Scene(this, "Test Scene");
        scene->OnInit();
        EntityHandle entity = scene->CreateEntity("Sprite");
        transform = entity->GetTransform();
        box = entity->AddComponent<BoxComponent2D>();
        box->SetType(PhysicsBodyType::Dynamic);
        box->SetGravityScale(1);

        EntityHandle floor = scene->CreateEntity("Floor");
        PlaneComponent2D* plane = floor->AddComponent<PlaneComponent2D>();
        plane->SetType(PhysicsBodyType::Static);
        plane->SetBodyPosition({ 0.0f, -1, 0.0f});

        renderer = entity->AddComponent<SpriteRenderer>();
        renderer->SetPixelsPerUnit(58*2);
        renderer->SetSpriteAnimation(playerIdleAnim);

        EntityHandle cameraEntity = scene->CreateEntity("Camera");
        cameraEntity->GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 1.0f));

        Camera* camera = cameraEntity->AddComponent<Camera>();
        camera->SetDimensions(renderTarget->GetWidth(), renderTarget->GetHeight());
        camera->SetClipPlanes(0.001f, 1000.0f);
        camera->SetProjectionMode(CameraProjectionMode::Orthographic);
        camera->SetOrthographicSize(300.0f);

        return scene;
    };

    SceneManager* sceneManager = GetSceneManager();
    sceneManager->LoadScene(constructScene());
}

void GameApplication::OnUpdate(float deltaTime)
{
    const auto& window = GetWindow().As<DesktopWindow>();
    if (window->GetKeyDown(KeyCode::Space))
    {
        const Vector3 velocity = Vector3::Up * Math::Sqrt(2.0f * 9.81 * 1.0f);
        box->SetLinearVelocity(velocity);
    }
}



void GameApplication::OnGUI()
{
    DrawFps(GetDeltaTime(), 3.0f);
    if (ImGui::Begin("Settings"))
    {
        DrawSpriteRenderer("Sprite Renderer", renderer);
        DrawTransform("Transform", transform);
        DrawBoxComponent("Box Component", box);
    }
    ImGui::End();
}

Ref<Texture> GameApplication::LoadTexture(const String& name, const StringView filepath)
{
    const Ref<Device>& device = GetDevice();

    stbi_set_flip_vertically_on_load(true);
    int32_t width = 0, height = 0;
    stbi_uc* pixels = stbi_load(*filepath, &width, &height, nullptr, STBI_rgb_alpha);
    if (!pixels) return nullptr;

    TextureCreateInfo createInfo;
    createInfo.device = device;
    createInfo.width = width;
    createInfo.height = height;
    createInfo.depth = 1;
    createInfo.format = Format::R8G8B8A8_UNORM;
    createInfo.mips = 1;
    createInfo.sampleCount = 1;
    createInfo.usageFlags = TextureUsageFlagBits::Sampled;
    createInfo.data = pixels;
    createInfo.dataSize = width * height * 4 * sizeof(stbi_uc);

    AssetDatabase& assetDatabase = GetAssetDatabase();
    Ref<Texture> texture = assetDatabase.CreateAsset<Vulkan::Texture>(name);
    if (!texture) return nullptr;
    if (!texture->Initialize(createInfo))
    {
        assetDatabase.UnloadAsset(texture);
        stbi_image_free(pixels);
        return nullptr;
    }

    stbi_image_free(pixels);
    return texture;
}

Ref<SpriteAnimation> GameApplication::LoadSpriteAnimation(const String& name, const SpriteSheet& spriteSheet)
{
    AssetDatabase& assetDatabase = GetAssetDatabase();
    Ref<SpriteAnimation> animation = assetDatabase.CreateAsset<SpriteAnimation>(name);
    if (!animation) return nullptr;
    if (!animation->LoadFromSpriteSheet(spriteSheet))
    {
        assetDatabase.UnloadAsset(animation);
        return nullptr;
    }
    return animation;
}
