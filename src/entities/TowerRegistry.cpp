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
        { //  Buy Cost,   Sell Cost,   Damage,   Range,        Fire Rate
            { 20,         5,           1,        200.f,        1.25f }, // Level 0 (1st level)
            { 50,         15,          2,        250.f,        1.10f }, // Level 1 (2nd level)
            { 90,         35,          3,        300.f,        0.90f }  // Level 2 (3rd level)
        }
        // Add more tower types here
    }
    };

    return registry;
}