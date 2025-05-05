// ================================================================================================
// File: SplashTower.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 5, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "SplashTower.hpp"

SplashTower::SplashTower(sf::Vector2i tilePosition) :
	Tower(TowerRegistry::Type::Splash, sf::Color(255, 75, 51), tilePosition)
{
	this->bulletSpeed = 300.f;
	this->bulletColor = sf::Color(123, 37, 25);
}

void SplashTower::update(float fixedTimeStep, std::vector<Enemy>& enemies)
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

		// Check if the bullet has reached its maximum range
		// If it has, explode at the current position and mark the bullet as having hit an enemy
		if (Utility::distance(bullet.positionCurrent, position) >= attributes.at(level).range)
		{
			explodeAt(bullet.positionCurrent, enemies);
			bullet.hasHitEnemy = true;
			continue;
		}

		for (auto& enemy : enemies)
		{
			if (!bullet.hasHitEnemy && Utility::distance(bullet.positionCurrent, enemy.getPixelPosition()) <= enemy.getSize())
			{
				explodeAt(bullet.positionCurrent, enemies);
				bullet.hasHitEnemy = true;
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
			// Predict target intercept position
			auto predictedPosOpt = Utility::predictTargetIntercept(
				position,
				target->getPixelPosition(),
				target->getVelocity(),
				bulletSpeed
			);

			if (predictedPosOpt.has_value())
				fireAt(predictedPosOpt.value());
			else
				fireAt(target->getPixelPosition()); // Fallback to current position if prediction fails

			// Add incoming splash damage to all enemies within predicted splash radius
			for (auto& enemy : enemies)
				if (Utility::distance(target->getPixelPosition(), enemy.getPixelPosition()) <= attributes.at(level).splashRadius)
					target->addIncomingDamage(attributes.at(level).damage);
		}
	}

	// Update explosion timers
	for (size_t i = 0; i < explosions.size(); ++i)
	{
		explosionTimers[i] += fixedTimeStep;
		float t = explosionTimers[i] / EXPLOSION_DURATION;

		if (t >= 1.f)
		{
			explosions.erase(explosions.begin() + i);
			explosionTimers.erase(explosionTimers.begin() + i);
			continue;
		}

		float radius = attributes.at(level).splashRadius * t;
		explosions[i].setRadius(radius);
		explosions[i].setOrigin({ radius, radius });
		int alpha = static_cast<int>((1.f - t) * explosionColor.a); // Fade out
		explosions[i].setFillColor(sf::Color(explosionColor.r, explosionColor.g, explosionColor.b, alpha));

		++i;
	}
}

void SplashTower::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (isRangeCircleVisible)
		window.draw(rangeCircle);

	window.draw(shape);

	for (auto& bullet : bullets)
	{
		bullet.shape.setPosition(Utility::interpolate(bullet.positionPrevious, bullet.positionCurrent, interpolationFactor));
		window.draw(bullet.shape);
	}
	for (const auto& explosion : explosions)
		window.draw(explosion);
}

void SplashTower::fireAt(sf::Vector2f target)
{
	Bullet bullet;

	bullet.positionCurrent = position;
	bullet.positionPrevious = position;

	bullet.direction = Utility::normalize(target - position);

	bullet.shape.setRadius(8.f);
	bullet.shape.setOrigin({ 8.f, 8.f });
	bullet.shape.setFillColor(bulletColor);

	bullets.push_back(bullet);

	timeSinceLastShot = 0.f;
}

void SplashTower::explodeAt(sf::Vector2f location, std::vector<Enemy>& enemies)
{
	for (auto& enemy : enemies)
	{
		if (Utility::distance(location, enemy.getPixelPosition()) <= attributes.at(level).splashRadius)
		{
			enemy.takeDamage(attributes.at(level).damage);
		}
	}
	sf::CircleShape explosion;
	explosion.setRadius(0.f);
	explosion.setOrigin({ 0.f, 0.f });
	explosion.setFillColor(explosionColor);
	explosion.setPosition(location);

	explosions.push_back(explosion);
	explosionTimers.push_back(0.f);
}