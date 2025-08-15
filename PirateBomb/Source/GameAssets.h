#pragma once
#include "Runtime/Asset.h"
#include "Runtime/Ref.h"

enum GameAssets
{
    // Textures
    TEX_PlayerIdle,
    TEX_PlayerRun,
    TEX_PlayerJump,
    TEX_PlayerFall,
    TEX_PlayerHit,
    TEX_PlayerGround,
    TEX_PlayerDeadHit,
    TEX_PlayerDeadGround,

    // Animations
    ANM_PlayerIdle,
    ANM_PlayerRun,
    ANM_PlayerJump,
    ANM_PlayerFall,
    ANM_PlayerHit,
    ANM_PlayerGround,
    ANM_PlayerDeadHit,
    ANM_PlayerDeadGround,
    GAMEASSETS_COUNT
};

inline Nova::Ref<Nova::Asset> g_GameAssets[GAMEASSETS_COUNT] = { nullptr };

inline Nova::String g_GameAssetNames[GAMEASSETS_COUNT]
{
    "TEX_PlayerIdle",
    "TEX_PlayerRun",
    "TEX_PlayerJump",
    "TEX_PlayerFall",
    "TEX_PlayerHit",
    "TEX_PlayerGround",
    "TEX_PlayerDeadHit",
    "TEX_PlayerDeadGround",
    "ANM_PlayerIdle",
    "ANM_PlayerRun",
    "ANM_PlayerJump",
    "ANM_PlayerFall",
    "ANM_PlayerHit",
    "ANM_PlayerGround",
    "ANM_PlayerDeadHit",
    "ANM_PlayerDeadGround",
};
