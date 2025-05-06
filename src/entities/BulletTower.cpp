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
	Tower(TowerRegistry::Type::Bullet, sf::Color(8, 74, 44), sf::Color(9, 83, 49), sf::Color(10, 92, 54), tilePosition)
{
	this->bulletSpeed = 900.f;
	this->bulletColor = sf::Color(5, 46, 27);
}

void BulletTower::update(float fixedTimeStep, std::vector<Enemy>& enemies, SoundManager& soundManager)
{
	timeSinceLastShot += fixedTimeStep;

	updateSelectionOutline();

	// Erase bullets that have hit enemies (deferred by 1 frame for smooth interpolation)
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[](const Bullet& bullet) { return bullet.hasHitEnemy; }), bullets.end());

	// Move bullets and check for collisions with enemies
	for (auto& bullet : bullets)
	{
		bullet.positionPrevious = bullet.positionCurrent;
		bullet.positionCurrent += bullet.direction * bulletSpeed * fixedTimeStep;

		for (auto& enemy : enemies)
		{
			if (Utility::distance(bullet.positionCurrent, enemy.getPixelPosition()) <= enemy.getSize())
			{
				enemy.takeDamage(attributes.at(level).damage);
				bullet.hasHitEnemy = true;
				soundManager.playSound(SoundManager::SoundID::ENEMY_HIT, 0.15f);
				break;
			}
		}
	}
	// Attempt to fire if ready
	if (canFire())
	{
		Enemy* target = Utility::getClosestEnemyInRange(position, enemies, attributes.at(level).range);

		// If a valid target is found
		if (target)
		{
			auto predictedPosOpt = Utility::predictTargetIntercept(
				position,
				target->getPixelPosition(),
				target->getVelocity(),
				bulletSpeed
			);

			// Predict target intercept position
			if (predictedPosOpt.has_value())
				fireAt(predictedPosOpt.value());
			else
				fireAt(target->getPixelPosition()); // Fallback to current position if prediction fails

			soundManager.playSound(SoundManager::SoundID::BULLET_SHOOT, 0.15f);

			// Add incoming splash damage to target enemy
			target->addIncomingDamage(attributes.at(level).damage);
		}
	}
}

void BulletTower::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (isRangeCircleVisible)
		window.draw(rangeCircle);

	window.draw(shape);
	if (level > 0)
		window.draw(shape2);
	if (level > 1)
		window.draw(shape3);

	for (auto& bullet : bullets)
	{
		bullet.shape.setPosition(Utility::interpolate(bullet.positionPrevious, bullet.positionCurrent, interpolationFactor));
		window.draw(bullet.shape);
	}
}

void BulletTower::fireAt(sf::Vector2f target)
{
	Bullet bullet;

	bullet.positionCurrent = position;
	bullet.positionPrevious = position;

	bullet.direction = Utility::normalize(target - position);

	bullet.shape.setRadius(5.f);
	bullet.shape.setOrigin({ bullet.shape.getRadius(), bullet.shape.getRadius() });
	bullet.shape.setFillColor(bulletColor);

	bullets.push_back(bullet);

	timeSinceLastShot = 0.f;
}