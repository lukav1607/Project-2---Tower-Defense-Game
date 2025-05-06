// ================================================================================================
// File: Enemy.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "Enemy.hpp"
#include "../core/Utility.hpp"

const float Enemy::BASE_SPEED = 60.f;
const int Enemy::BASE_HEALTH = 5;

Enemy::Enemy(sf::Vector2i spawnTile, float speed, int health) :
	defaultColor(sf::Color(71, 28, 28)),
	size(15.f),
	direction(1.f, 0.f),
	baseSpeed(speed),
	currentSpeed(speed),
	damageFlashTimer(0.f),
	damageFlashDuration(0.1f),
	flashColor(sf::Color::White),
	health(health),
	incomingDamage(0),
	m_hasReachedEnd(false),
	isRunningDeathEffect(false)
{
	positionCurrent = Utility::tileToPixelPosition(spawnTile.x, spawnTile.y);
	positionCurrent.x -= size + Grid::TILE_SIZE;
	positionPrevious = positionCurrent;

	previousTile = Utility::pixelToTilePosition(positionCurrent);

	float rawValue = health * 0.6f + speed * 0.4f;
	worth = std::clamp(static_cast<int>(rawValue / 30.f), 1, 15);

	shape.setRadius(size);
	shape.setFillColor(defaultColor);
	shape.setOrigin({ size, size });
	shape.setPosition(positionCurrent);
}

void Enemy::update(float fixedTimeStep, const Grid& grid)
{
	sf::Vector2i currentTile = Utility::pixelToTilePosition(positionCurrent);

	// Update movement direction
	if (currentTile != previousTile)
	{
		sf::Vector2f centerOfCurrentTile = Utility::tileToPixelPosition(currentTile);

		// Snap to center if we passed it
		if (isPastCenterOfTile(centerOfCurrentTile))
		{
			positionCurrent = centerOfCurrentTile;
			previousTile = currentTile;

			if (isTileToRightPathable(grid))
				direction = { 1.f, 0.f };
			else if (direction != sf::Vector2f(0.f, -1.f) && isTileBelowPathable(grid))
				direction = { 0.f, 1.f };
			else if (direction != sf::Vector2f(0.f, 1.f) && isTileAbovePathable(grid))
				direction = { 0.f, -1.f };
		}
	}

	updateStatusEffects(fixedTimeStep);
	resetIncomingDamage();

	// Update damage flash effect
	if (damageFlashTimer > 0.f)
	{
		damageFlashTimer -= fixedTimeStep;
		if (damageFlashTimer < 0.f)
			damageFlashTimer = 0.f;
	}

	// Update death effect
	for (auto& effect : deathEffects)
		effect.update(fixedTimeStep);

	deathEffects.erase(std::remove_if(deathEffects.begin(), deathEffects.end(),
		[](const DeathEffect& e) { return e.isExpired(); }), deathEffects.end());

	if (isRunningDeathEffect && deathEffects.empty())
		isRunningDeathEffect = false;

	// Update position
	positionPrevious = positionCurrent;
	positionCurrent += direction * currentSpeed * fixedTimeStep;
	if (positionCurrent.x >= (grid.getSize().x * Grid::TILE_SIZE) + size )
		m_hasReachedEnd = true;
}

void Enemy::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (damageFlashTimer > 0.f)
	{
		float t = damageFlashTimer / damageFlashDuration;
		sf::Color blend = flashColor;
		blend.a = static_cast<std::uint8_t>(255 * t);
		shape.setFillColor(blend);
	}
	else
	{
		shape.setFillColor(currentColor);
	}
	
	for (auto& effect : deathEffects)
		effect.render(interpolationFactor, window);

	if (!isRunningDeathEffect)
	{
		shape.setPosition(Utility::interpolate(positionPrevious, positionCurrent, interpolationFactor));
		window.draw(shape);
	}
}

void Enemy::applyStatusEffect(const StatusEffect& effect)
{
	// Check if the same effect type is already applied
	for (auto& existingEffect : statusEffects)
	{
		if (existingEffect.type == effect.type)
		{
			// If the effect is already applied, update its properties
			existingEffect.amount = effect.amount;
			existingEffect.duration = effect.duration;
			existingEffect.timer = effect.timer;
			return;
		}
	}
	// If the effect isn't applied, apply it
	statusEffects.push_back(effect);
}

void Enemy::updateStatusEffects(float fixedTimeStep)
{
	float slowFactor = 1.f;
	sf::Color overlayColor = defaultColor;

	// Update status effects
	for (auto it = statusEffects.begin(); it != statusEffects.end();)
	{
		it->timer += fixedTimeStep;

		if (it->timer >= it->duration)
		{
			it = statusEffects.erase(it);
			continue;
		}
		if (it->type == StatusEffect::Type::Slow)
		{
			overlayColor = it->overlayColor;
			float thisSlowFactor = 1.f - it->amount;
			slowFactor = std::min(slowFactor, thisSlowFactor);
		}
		++it;
	}

	currentColor = Utility::blendColors(defaultColor, overlayColor);
	shape.setFillColor(currentColor);
	currentSpeed = baseSpeed * slowFactor;
}

void Enemy::takeDamage(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		health = 0;
		startDeathEffect();
	}

	damageFlashTimer = damageFlashDuration;
}

void Enemy::startDeathEffect()
{
	isRunningDeathEffect = true;

	int amount = Utility::randomNumber(4, 6);

	for (int i = 0; i < amount; ++i)
	{
		int size = Utility::randomNumber(3, 6);

		DeathEffect deathEffect;
		deathEffect.shape.setRadius(static_cast<float>(size));
		deathEffect.shape.setOrigin({ static_cast<float>(size), static_cast<float>(size) });
		deathEffect.shape.setFillColor(currentColor);
		deathEffect.shape.setPosition(positionCurrent);
		deathEffect.positionCurrent = positionCurrent;
		deathEffect.positionPrevious = positionCurrent;

		float angle = Utility::randomNumber(0.f, 360.f);
		float speed = Utility::randomNumber(50.f, 100.f);
		deathEffect.velocity = Utility::angleToVector(angle) * speed;

		deathEffect.lifetime = Utility::randomNumber(0.4f, 0.6f);
		deathEffects.push_back(deathEffect);
	}
}

bool Enemy::isPastCenterOfTile(sf::Vector2f center) const
{
	return (direction.x != 0 && ((direction.x > 0 && positionCurrent.x >= center.x) || (direction.x < 0 && positionCurrent.x <= center.x))) ||
		   (direction.y != 0 && ((direction.y > 0 && positionCurrent.y >= center.y) || (direction.y < 0 && positionCurrent.y <= center.y)));
}

bool Enemy::isTileToRightPathable(const Grid& grid) const
{
	return grid.getTileType(Utility::pixelToTilePosition(positionCurrent) + sf::Vector2i(1, 0)) == Tile::Type::Pathable ||
		   grid.getTileType(Utility::pixelToTilePosition(positionCurrent) + sf::Vector2i(1, 0)) == Tile::Type::End;
}

bool Enemy::isTileAbovePathable(const Grid& grid) const
{
	return grid.getTileType(Utility::pixelToTilePosition(positionCurrent) + sf::Vector2i(0, -1)) == Tile::Type::Pathable ||
		   grid.getTileType(Utility::pixelToTilePosition(positionCurrent) + sf::Vector2i(0, -1)) == Tile::Type::End;
}

bool Enemy::isTileBelowPathable(const Grid& grid) const
{
	return grid.getTileType(Utility::pixelToTilePosition(positionCurrent) + sf::Vector2i(0, 1)) == Tile::Type::Pathable ||
		   grid.getTileType(Utility::pixelToTilePosition(positionCurrent) + sf::Vector2i(0, 1)) == Tile::Type::End;
}
