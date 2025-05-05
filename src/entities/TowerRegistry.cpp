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
            { 20,         8,           2,        240.f,   1.f,         0.f,             0.f,           0.f }, // Level 0
            { 50,         17,          5,        300.f,   0.9f,        0.f,             0.f,           0.f }, // Level 1
            { 90,         30,          8,        360.f,   0.8f,        0.f,             0.f,           0.f }  // Level 2
        }
    },
    {
        Type::Splash,
        "Splash",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 30,         10,          1,        150.f,   2.f,         60.f,           0.f,            0.f }, // Level 0
            { 70,         22,          3,        200.f,   1.75f,       90.f,           0.f,            0.f }, // Level 1
            { 120,        40,          5,        250.f,   1.5f,        120.f,          0.f,            0.f }  // Level 2
        }
    },
    {
        Type::Slow,
        "Slow",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate,   Splash Radius,   Slow Amount,   Slow Duration
            { 40,         12,          0,        300.f,   4.f,         0.f,             0.6f,          1.5f }, // Level 0
            { 90,         30,          0,        350.f,   3.75f,       0.f,             0.7f,          2.f },  // Level 1
            { 150,        50,          0,        400.f,   3.5f,        0.f,             0.8f,          2.5f }  // Level 2
        }
    }
    };

    return registry;
}