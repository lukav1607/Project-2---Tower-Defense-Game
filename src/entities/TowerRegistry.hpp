// ================================================================================================
// File: TowerRegistry.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// Description: Defines the TowerRegistry namespace, which contains a registry of tower metadata.
//              This includes information about each tower type, such as its name, damage, fire rate,
//              range, and buy cost. The registry is used to initialize tower attributes and provide
//              information to the user interface.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <vector>
#include <string>

namespace TowerRegistry
{
    enum class Type
    {
        Bullet,
        Slow,
        Count
    };

    struct AttributesPerLevel
    {
		// General attributes used by most towers
        int buyCost;
        int sellCost;
        int damage;
        float range;
        float fireRate;
		// Used by Slow Towers
        float slowAmount;      // e.g., 0.5f means -50% speed
        float slowDuration;    // in seconds
    };

    struct TowerMetadata
    {
        Type type;
        std::string name;
		std::vector<AttributesPerLevel> attributes;
    };

    const std::vector<TowerMetadata>& getTowerMetadataRegistry();
}