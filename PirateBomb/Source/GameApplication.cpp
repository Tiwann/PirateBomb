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
#include "Components/Physics/PlaneComponent2D.h"
#include "GameAssets.h"
#include "PlayerController.h"
#include "External/stb_image.h"
#include <cstdint>
#include <imgui.h>


NOVA_DEFINE_APPLICATION_CLASS(GameApplication)

using namespace Nova;
using namespace Nova::Rendering;

static Transform* transform = nullptr;
static SpriteRenderer* renderer = nullptr;
static BoxComponent2D* box = nullptr;
static PlaneComponent2D* plane;

ApplicationConfiguration GameApplication::GetConfiguration() const
{
    ApplicationConfiguration config;
    config.windowWidth = 800;
    config.windowHeight = 600;
    config.vsync = true;
    config.applicationName = "Pirate Bomb";
    config.fullscreen = false;
    config.resizable = false;
    return config;
}

void GameApplication::OnInit()
{
    LoadTexture(TEX_PlayerIdle, "Sprites/Player-Idle-58.png");
    LoadTexture(TEX_PlayerRun, "Sprites/Player-Run-58.png");
    LoadTexture(TEX_PlayerJump, "Sprites/Player-Jump-58.png");
    LoadTexture(TEX_PlayerFall, "Sprites/Player-Fall-58.png");
    LoadTexture(TEX_PlayerHit, "Sprites/Player-Hit-58.png");
    LoadTexture(TEX_PlayerGround, "Sprites/Player-Ground-58.png");
    LoadTexture(TEX_PlayerDeadHit, "Sprites/Player-DeadHit-58.png");
    LoadTexture(TEX_PlayerDeadGround, "Sprites/Player-DeadGround-58.png");

    LoadAnimation(TEX_PlayerIdle, ANM_PlayerIdle, 2, 13, 26, 58);
    LoadAnimation(TEX_PlayerRun, ANM_PlayerRun, 2, 7, 14, 58);
    LoadAnimation(TEX_PlayerJump, TEX_PlayerJump, 1, 4, 4, 58);
    LoadAnimation(TEX_PlayerFall, ANM_PlayerFall, 1, 2, 2, 58);
    LoadAnimation(TEX_PlayerHit, ANM_PlayerHit, 2, 4, 8, 58);
    LoadAnimation(TEX_PlayerGround, ANM_PlayerGround, 1, 3, 3, 58);
    LoadAnimation(TEX_PlayerDeadHit, ANM_PlayerDeadHit, 1, 6, 6, 58);
    LoadAnimation(TEX_PlayerDeadGround, ANM_PlayerDeadGround, 1, 4, 4, 58);

    const auto constructScene = [this] () {
        const RenderTarget* renderTarget = GetRenderTarget();

        Scene* scene = new Scene(this, "Test Scene");
        scene->OnInit();

        EntityHandle entity = scene->CreateEntity("Sprite");
        transform = entity->GetTransform();
        box = entity->AddComponent<BoxComponent2D>();
        box->SetBodyPosition(Vector3::Up * 100.0f);
        box->SetType(PhysicsBodyType::Dynamic);
        box->SetGravityScale(1.0f);
        box->SetConstraints(PhysicsConstraintsFlagBits::Rotation);


        entity->AddComponent<PlayerController>();

        EntityHandle floor = scene->CreateEntity("Floor");
        plane = floor->AddComponent<PlaneComponent2D>();
        plane->SetType(PhysicsBodyType::Static);
        plane->SetBodyPosition(Vector3::Down * 1.0f);

        EntityHandle newEntity = scene->CreateEntity("New Entity");
        newEntity->SetParent(entity.GetEntity());
#if 0
        renderer = entity->AddComponent<SpriteRenderer>();
        renderer->SetPixelsPerUnit(58*2);
        renderer->SetSpriteAnimation(g_GameAssets[ANM_PlayerIdle]);
#endif

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

void GameApplication::OnUpdate(const float deltaTime)
{
    Application::OnUpdate(deltaTime);
    if (const Ref<DesktopWindow> input = GetWindow().As<DesktopWindow>(); input->GetKeyDown(KeyCode::F11))
        m_EnableGui = !m_EnableGui;
}

void GameApplication::OnDrawDebug()
{
}

void GameApplication::OnGUI()
{
    if (m_EnableGui)
    {
        DrawFps(GetDeltaTime(), 3.0f);
        if (ImGui::Begin("Settings"))
        {
            DrawSpriteRenderer("Sprite Renderer", renderer);
            DrawTransform("Transform", transform);
            DrawBoxComponent2D("Box Component", box);
            DrawPlaneComponent2D("Plane Component", plane);
        }
        ImGui::End();
    }
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

void GameApplication::LoadTexture(const GameAssets asset, const StringView filepath)
{
    g_GameAssets[asset] = LoadTexture(g_GameAssetNames[asset], Path::GetAssetPath(filepath));
}

void GameApplication::LoadAnimation(const GameAssets texture, const GameAssets animation, const uint32_t rows, const uint32_t columns, const uint32_t spriteCount, const uint32_t spriteSize)
{
    g_GameAssets[animation] = LoadSpriteAnimation(g_GameAssetNames[texture], SpriteSheet(rows, columns, spriteCount, spriteSize, g_GameAssets[texture].As<Texture>()));
}
