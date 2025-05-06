// ================================================================================================
// File: SoundManager.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 6, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "SoundManager.hpp"
#include "../core/Utility.hpp"

using namespace Utility;

void SoundManager::loadSounds()
{
	auto load = [&](SoundID soundID, const std::string& filename)
		{
			soundBuffers[soundID] = std::make_shared<sf::SoundBuffer>(filename);
		};

	load(SoundID::BULLET_SHOOT, "assets/sounds/bullet-shoot.wav");
	load(SoundID::SPLASH_SHOOT, "assets/sounds/splash-shoot.wav");
	load(SoundID::SPLASH_EXPLODE, "assets/sounds/splash-explosion.wav");
	load(SoundID::SLOW_PULSE, "assets/sounds/slow-pulse.wav");
	load(SoundID::LIFE_LOST, "assets/sounds/life-lost.wav");
	load(SoundID::NEW_WAVE, "assets/sounds/new-wave.wav");
	load(SoundID::ENEMY_HIT, "assets/sounds/enemy-hit.wav");
	load(SoundID::ENEMY_DEATH, "assets/sounds/enemy-death.wav");
	load(SoundID::TOWER_UPGRADE, "assets/sounds/tower-upgrade.wav");
	load(SoundID::BUTTON_CLICK, "assets/sounds/button-click.wav");
}

void SoundManager::playSound(SoundID soundID, float pitchVariancePercentage, float volumeMultiplier)
{
	auto it = soundBuffers.find(soundID);
	if (it != soundBuffers.end())
	{
		auto sound = std::make_shared<sf::Sound>(*it->second);

		if (pitchVariancePercentage < 0.f || pitchVariancePercentage > 1.f) {
			std::cerr << "Warning: Pitch variance percentage must be between 0 and 1! Pitch variance set to default (0%)" << std::endl;
			pitchVariancePercentage = 0.f;
		}
		if (pitchVariancePercentage != 0.f)
			sound->setPitch(Utility::randomPitch(pitchVariancePercentage));

		sound->setVolume(volume * volumeMultiplier);
		sound->play();

		activeSounds.push_back(sound);
	}
	else
	{
		std::cerr << "Error: Sound ID not found!" << std::endl;
	}
}

void SoundManager::cleanupSounds()
{
	activeSounds.erase(
		std::remove_if(activeSounds.begin(), activeSounds.end(),
			[](const std::shared_ptr<sf::Sound>& sound) {
				return sound->getStatus() == sf::Sound::Status::Stopped;
			}),
		activeSounds.end());
}
