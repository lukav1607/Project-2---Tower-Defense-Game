// ================================================================================================
// File: Enemy.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// Description: Defines the Enemy class, which represents an enemy unit in the game. The class contains
//              information about the enemy's position, health, speed, and other attributes. It also
//              includes methods for updating the enemy's state and rendering it on the screen.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "DeathEffect.hpp"

class Enemy : public Entity
{
public:
	struct StatusEffect
	{
		enum class Type
		{
			Slow
		};
		inline StatusEffect(StatusEffect::Type type, sf::Color overlayColor, float amount, float duration) :
			type(type),
			overlayColor(overlayColor),
			amount(amount),
			duration(duration)
		{}
		Type type;
		sf::Color overlayColor;
		float amount;     // e.g., 0.5 for -50% speed
		float duration;   // in seconds
		float timer = 0.f;
	};

	Enemy() = default;
	Enemy(sf::Vector2i spawnTile, float speed, int health);
	Enemy(const Enemy&) = default;
	Enemy& operator=(const Enemy&) = default;
	~Enemy() override = default;

	void update(float fixedTimeStep, const Grid& grid) override;
	void render(float interpolationFactor, sf::RenderWindow& window) override;

	void applyStatusEffect(const StatusEffect& effect);
	void updateStatusEffects(float fixedTimeStep);

	void takeDamage(int damage);
	inline void addIncomingDamage(int dmg) { incomingDamage += dmg; }
	inline void resetIncomingDamage() { incomingDamage = 0; }
	inline int getIncomingDamage() const { return incomingDamage; }

	inline bool isDead() const { return health <= 0 && !isRunningDeathEffect; }
	inline bool hasReachedEnd() const { return m_hasReachedEnd; }
	inline bool hasStartedPath() const { return positionCurrent.x > 0; }
	inline int getHealth() const { return health; }
	inline int getWorth() const { return worth; }
	inline sf::Vector2f getPixelPosition() const { return positionCurrent; }
	inline sf::Vector2f getVelocity() const { return direction * currentSpeed; }
	inline float getSize() const { return size; }

	static const float BASE_SPEED;
	static const int BASE_HEALTH;

private:
	void startDeathEffect();

	bool isPastCenterOfTile(sf::Vector2f center) const;
	bool isTileToRightPathable(const Grid& grid) const;
	bool isTileAbovePathable(const Grid& grid) const;
	bool isTileBelowPathable(const Grid& grid) const;

	sf::CircleShape shape;
	sf::Color defaultColor;
	sf::Color currentColor;
	float size;

	std::vector<StatusEffect> statusEffects;
	float baseSpeed;
	float currentSpeed;

	float damageFlashTimer ;
	float damageFlashDuration;
	sf::Color flashColor;

	sf::Vector2i previousTile;
	sf::Vector2f direction;
	bool m_hasReachedEnd;

	int health;
	int incomingDamage;
	int worth;

	std::vector<DeathEffect> deathEffects;
	bool isRunningDeathEffect;
};