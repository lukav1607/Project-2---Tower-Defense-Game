// ================================================================================================
// File: BulletTower.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "BulletTower.hpp"
#include "../core/Utility.hpp"

BulletTower::BulletTower(sf::Vector2i tilePosition) :
	Tower(TowerRegistry::Type::Bullet, sf::Color(10, 92, 54), tilePosition),
	bulletColor(sf::Color(25, 17, 6))
{}

void BulletTower::update(float fixedTimeStep, std::vector<Enemy>& enemies)
{
	timeSinceLastShot += fixedTimeStep;

	// Erase bullets that have hit enemies (deferred by 1 frame for smooth interpolation)
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[](const Bullet& bullet) { return bullet.hasHitEnemy; }), bullets.end());

	// Move bullets and check for collisions with enemies
	for (auto& bullet : bullets)
	{
		bullet.positionPrevious = bullet.positionCurrent;
		bullet.positionCurrent += bullet.direction * bullet.SPEED * fixedTimeStep;

		for (auto& enemy : enemies)
		{
			if (Utility::distance(bullet.positionCurrent, enemy.getPixelPosition()) <= enemy.getSize())
			{
				enemy.takeDamage(attributes.at(level).damage);
				bullet.hasHitEnemy = true;
				break;
			}
		}
	}

	// Attempt to fire if ready
	if (canFire())
	{
		Enemy* bestTarget = nullptr;
		float closestDistanceSq = std::numeric_limits<float>::max();

		for (auto& enemy : enemies)
		{
			float distanceSq = Utility::distanceSquared(position, enemy.getPixelPosition());
			float rangeSq = attributes.at(level).range * attributes.at(level).range;

			if (distanceSq < rangeSq)
			{
				if (distanceSq < closestDistanceSq)
				{
					bestTarget = &enemy;
					closestDistanceSq = distanceSq;
				}
			}
		}
		if (bestTarget)
		{
			auto predictedPosOpt = Utility::predictTargetIntercept(
				position,
				bestTarget->getPixelPosition(),
				bestTarget->getVelocity(),
				Bullet::SPEED
			);

			if (predictedPosOpt.has_value())
				fireAt(predictedPosOpt.value());
			else
				fireAt(bestTarget->getPixelPosition()); // Fallback to current position if prediction fails
		}
	}
}

void BulletTower::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (isSelected)
	{
		shape.setOutlineColor(sf::Color(255, 255, 255, 255));
		shape.setOutlineThickness(4.f);
	}
	else
	{
		shape.setOutlineColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(0.f);
	}

	if (isRangeCircleVisible)
		window.draw(rangeCircle);

	window.draw(shape);

	for (auto& bullet : bullets)
	{
		bullet.shape.setPosition(Utility::interpolate(bullet.positionPrevious, bullet.positionCurrent, interpolationFactor));
		window.draw(bullet.shape);
	}
}

void BulletTower::fireAt(sf::Vector2f target)
{
	if (target.x <= 0)
		return;

	Bullet bullet;
	bullet.positionCurrent = position;
	bullet.positionPrevious = position;

	sf::Vector2f direction = Utility::normalize(target - position);
	bullet.direction = direction;

	bullet.shape.setRadius(5.f);
	bullet.shape.setOrigin({ bullet.shape.getRadius(), bullet.shape.getRadius() });
	bullet.shape.setFillColor(bulletColor);

	bullets.push_back(bullet);
	timeSinceLastShot = 0.f;
}