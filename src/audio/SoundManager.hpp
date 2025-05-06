// ================================================================================================
// File: SoundManager.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 6, 2025
// Description: Defines the SoundManager class, which is responsible for managing sound effects in the game.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <unordered_map>
#include <SFML/Audio.hpp>

class SoundManager
{
public:
	enum class SoundID
	{
		BULLET_SHOOT,
		SPLASH_SHOOT,
		SPLASH_EXPLODE,
		SLOW_PULSE,
		LIFE_LOST,
		NEW_WAVE,
		ENEMY_HIT,
		ENEMY_DEATH,
		TOWER_UPGRADE,
		BUTTON_CLICK
	};

	void loadSounds();

	// Remove any sounds that are no longer playing
	void cleanupSounds();

	// Play a sound at specified volume with a random pitch variation.
	// Note: pitchVariancePercentage 0.15f == +/- 15% variation
	void playSound(SoundID soundID, float pitchVariancePercentage = 0.f, float volumeMultiplier = 1.f);

	float volume = 100.f;

private:
	std::unordered_map<SoundID, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
	std::vector<std::shared_ptr<sf::Sound>> activeSounds;
};