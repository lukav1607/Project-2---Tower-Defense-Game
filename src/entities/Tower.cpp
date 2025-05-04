// ================================================================================================
// File: Tower.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// Description: Defines the Tower class, which is a base class for all tower types in the game.
//              It contains common attributes and methods for all tower types.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Tower.hpp"

Tower::Tower(TowerRegistry::Type type, sf::Vector2i tilePosition) :
	isSelected(false),
	type(type),
	position(Utility::tileToPixelPosition(tilePosition)),
	timeSinceLastShot(0.f),
	level(0),
	m_isMarkedForSale(false),
	m_isMarkedForUpgrade(false),
	isRangeCircleVisible(false)
{
	// Fetch the tower metadata attributes from the registry
	const TowerRegistry::TowerMetadata& metadata = TowerRegistry::getTowerMetadataRegistry()[static_cast<int>(type)];
	attributes = metadata.attributes;
}