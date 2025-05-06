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
            { 75,         40,          4,        300.f,   1.f,         0.f,             0.f,           0.f }, // Level 1
            { 150,        100,         6,        360.f,   0.85f,       0.f,             0.f,           0.f }  // Level 2
        }
    },
    {
        Type::Splash,
        "Splash",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 30,         15,          1,        160.f,   2.2f,        60.f,           0.f,            0.f }, // Level 0
            { 100,        55,          2,        200.f,   1.9f,        80.f,           0.f,            0.f }, // Level 1
            { 200,        130,         4,        240.f,   1.6f,        110.f,          0.f,            0.f }  // Level 2
        }
    },
    {
        Type::Slow,
        "Slow",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 25,         15,          0,        280.f,   4.f,         0.f,             0.4f,          1.5f },  // Level 0
            { 90,         50,          0,        340.f,   3.75f,       0.f,             0.5f,          1.75f }, // Level 1
            { 180,        125,         0,        400.f,   3.5f,        0.f,             0.6f,          2.f }    // Level 2
        }
    }
    };

    return registry;
}