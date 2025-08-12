cmake_minimum_required(VERSION 4.0)

project(PirateBomb)

# include nova engine
set(NOVA_ENGINE_BUILD_EXAMPLES OFF)
set(NOVA_ENGINE_CUSTOM_ENTRY_POINT OFF)
set(NOVA_ENGINE_INCLUDE_AUDIO ON)
set(NOVA_ENGINE_INCLUDE_PHYSICS ON)
add_subdirectory(Vendor/NovaEngine)

#include pirate bomb
add_subdirectory(PirateBomb)