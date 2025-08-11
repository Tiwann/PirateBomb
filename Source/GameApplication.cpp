#include "GameApplication.h"
#include "Rendering/Texture.h"
#include "Rendering/Vulkan/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"

#include <cstdint>


namespace Nova
{
    extern "C" Application* CreateApplication() { return new GameApplication(); }
}

using namespace Nova;
using namespace Nova::Rendering;

void GameApplication::OnInit()
{

}

void GameApplication::OnUpdate(float deltaTime)
{
    Application::OnUpdate(deltaTime);
}

Texture* GameApplication::LoadTexture(const String& name, const StringView filepath)
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
    Vulkan::Texture* texture = assetDatabase.CreateAsset<Vulkan::Texture>(name);
    if (!texture) return nullptr;
    if (!texture->Initialize(createInfo)) return nullptr;
    stbi_image_free(pixels);
    return texture;
}
