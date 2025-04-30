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
	position(Utility::tileToPixelPosition(tilePosition)),
	timeSinceLastShot(0.f),
	level(1)
{
	shape.setSize({ 50.f, 50.f });
	shape.setFillColor(sf::Color(200, 200, 200));
	shape.setOrigin({ shape.getSize().x / 2.f, shape.getSize().y / 2.f });
	shape.setPosition(position);
}

void Tower::update(float fixedTimeStep, std::vector<Enemy>& enemies)
{
	timeSinceLastShot += fixedTimeStep;

	// Erase bullets that have hit an enemy only at the start of the next update.
	// This is done to avoid visual issues where the bullet's interpolated position is not
	// the same as the position it was at when it logically hit the enemy. This gives the
	// rendering interpolation enough time to catch up with the logic.
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[](const Bullet& bullet) { return bullet.hasHitEnemy; }), bullets.end());

	for (auto& bullet : bullets)
	{
		bullet.positionPrevious = bullet.positionCurrent;
		bullet.positionCurrent += bullet.direction * bullet.speed * fixedTimeStep;

		for (auto& enemy : enemies)
		{
			if (Utility::distance(bullet.positionCurrent, enemy.getPixelPosition()) <= enemy.getSize())
			{
				enemy.takeDamage(attributes[level - 1].damage);
				bullet.hasHitEnemy = true;
				break;
			}
		}
	}
}

void Tower::render(float interpolationFactor, sf::RenderWindow& window)
{
	window.draw(shape);
	for (auto& bullet : bullets)
	{
		bullet.shape.setPosition(Utility::interpolate(bullet.positionPrevious, bullet.positionCurrent, interpolationFactor));
		window.draw(bullet.shape);
	}
}

void Tower::fireAt(sf::Vector2f target)
{
	Bullet bullet;
	bullet.positionCurrent = position;
	bullet.positionPrevious = position;

	sf::Vector2f direction = Utility::normalize(target - position);
	bullet.direction = direction;
	bullet.speed = 1000.f;

	bullet.shape.setRadius(5.f);
	bullet.shape.setOrigin({ bullet.shape.getRadius(), bullet.shape.getRadius() });
	bullet.shape.setFillColor(sf::Color(255, 0, 0));

	bullets.push_back(bullet);

	timeSinceLastShot = 0.f;
}

Tower::Bullet::Bullet() :
	hasHitEnemy(false),
	speed(1000.f)
{}