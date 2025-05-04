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
        "Bullet Tower",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate   Slow Amount   Slow Duration
            { 20,         5,           1,        200.f,   1.25f,      0.f,          0.f}, // Level 0 (1st level)
            { 50,         15,          2,        250.f,   1.10f,      0.f,          0.f}, // Level 1 (2nd level)
            { 90,         35,          3,        300.f,   0.90f,      0.f,          0.f}  // Level 2 (3rd level)
        }
    },
    {
        Type::Slow,
        "Slow Tower",
        { //  Buy Cost,   Sell Cost,   Damage,   Range,   Fire Rate   Slow Amount   Slow Duration
            { 40,         10,          0,        300.f,   4.f,        0.6f,         1.5f}, // Level 0 (1st level)
            { 90,         25,          0,        400.f,   3.75f,      0.7f,         2.f}, // Level 1 (2nd level)
            { 150,        50,          0,        500.f,   3.5f,       0.8f,         2.5f}  // Level 2 (3rd level)
        }
    }
    };

    return registry;
}