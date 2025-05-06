// ================================================================================================
// File: TowerRegistry.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "TowerRegistry.hpp"

const std::vector<TowerRegistry::TowerMetadata>& TowerRegistry::getTowerMetadataRegistry()
{
    static std::vector<TowerRegistry::TowerMetadata> registry = {
    {
        Type::Bullet,
        "Bullet",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 20,         10,          2,        240.f,   1.1f,        0.f,             0.f,           0.f }, // Level 0
            { 75,         30,          4,        300.f,   0.95f,       0.f,             0.f,           0.f }, // Level 1
            { 150,        100,         6,        360.f,   0.8f,        0.f,             0.f,           0.f }  // Level 2
        }
    },
    {
        Type::Splash,
        "Splash",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 30,         15,          1,        160.f,   2.2f,        60.f,           0.f,            0.f }, // Level 0
            { 100,        50,          2,        200.f,   1.9f,        90.f,           0.f,            0.f }, // Level 1
            { 200,        125,         4,        240.f,   1.6f,        120.f,          0.f,            0.f }  // Level 2
        }
    },
    {
        Type::Slow,
        "Slow",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 30,         15,          0,        300.f,   4.f,         0.f,             0.5f,          1.5f },  // Level 0
            { 100,        20,          0,        350.f,   3.75f,       0.f,             0.55f,         1.75f }, // Level 1
            { 200,        125,         0,        400.f,   3.5f,        0.f,             0.6f,          2.f }    // Level 2
        }
    }
    };

    return registry;
}