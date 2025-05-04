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

const float Enemy::BASE_SPEED = 70.f;
const int Enemy::BASE_HEALTH = 2;

Enemy::Enemy(sf::Vector2i spawnTile, float speed, int health) :
	size(15.f),
	direction(1.f, 0.f),
	speed(speed),
	health(health),
	m_hasReachedEnd(false)
{
	positionCurrent = Utility::tileToPixelPosition(spawnTile.x, spawnTile.y);
	positionCurrent.x -= size + Grid::TILE_SIZE;
	positionPrevious = positionCurrent;

	previousTile = Utility::pixelToTilePosition(positionCurrent);

	worth = std::floorf((speed * health / 2.f) / 50.f);

	shape.setRadius(size);
	shape.setFillColor(sf::Color(71, 28, 28));
	shape.setOrigin({ size, size });
	shape.setPosition(positionCurrent);
}

void Enemy::update(float fixedTimeStep, const Grid& grid)
{
	sf::Vector2i currentTile = Utility::pixelToTilePosition(positionCurrent);

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

	positionPrevious = positionCurrent;
	positionCurrent += direction * speed * fixedTimeStep;

	if (positionCurrent.x >= (grid.getSize().x * Grid::TILE_SIZE) + size )
	{
		m_hasReachedEnd = true;
		std::cout << "Enemy reached the end!" << std::endl;
	}
}

void Enemy::render(float interpolationFactor, sf::RenderWindow& window)
{
	shape.setPosition(Utility::interpolate(positionPrevious, positionCurrent, interpolationFactor));
	window.draw(shape);
}

void Enemy::takeDamage(int damage)
{
	health -= damage;
	if (health <= 0)
		health = 0;
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
