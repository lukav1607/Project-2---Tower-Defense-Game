// ================================================================================================
// File: Tower.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Tower.hpp"
#include "../core/Utility.hpp"

Tower::Tower(Type type, sf::Vector2i tilePosition) :
	type(type),
	damage(1),
	range(100.f),
	fireRate(1.f),
	level(1),
	cost(10),
	position(Utility::tileToPixelPosition(tilePosition))
{
	shape.setSize({ 50.f, 50.f });
	shape.setFillColor(sf::Color(200, 200, 200));
	shape.setOrigin({ shape.getSize().x / 2.f, shape.getSize().y / 2.f });
	shape.setPosition(position);
}

void Tower::update(float fixedTimeStep)
{
	for (auto& bullet : bullets)
	{
		bullet.positionPrevious = bullet.positionCurrent;
		bullet.positionCurrent += bullet.direction * bullet.speed * fixedTimeStep;
	}
}

void Tower::render(float interpolationFactor, sf::RenderWindow& window)
{
}
