// ================================================================================================
// File: Entity.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// Description: Defines the Entity class, which serves as a base class for all game entities.
//              The class contains common attributes and methods that can be shared among different
//              types of entities, such as enemies and towers. This includes position, health,
//              and methods for updating and rendering the entity.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "../core/Grid.hpp"

class Entity
{
public:
	Entity() = default;
	Entity(const Entity&) = default;
	Entity& operator=(const Entity&) = default;
	virtual ~Entity() = default;

	virtual void update(float fixedTimeStep, const Grid& grid) = 0;
	virtual void render(float interpolationFactor, sf::RenderWindow& window) = 0;

protected:
	sf::Vector2f positionCurrent;
	sf::Vector2f positionPrevious;
};