// ================================================================================================
// File: HUD.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "HUD.hpp"

HUD::HUD(const sf::Font& font, sf::Vector2u windowSize) :
	lives(0),
	gold(0),
	wave(0),
	livesText(font, "Level: N/A", 48U),
	goldText(font, "Gold: N/A", 48U),
	waveText(font, "Wave: N/A", 48U),
	infoText(
		font,
		"Right click a tile to choose a tower to buy.\nRight click a tower to see tower info menu.\nDestroy enemies before they reach the end!",
		40U)
{
	livesText.setFillColor(sf::Color(220, 223, 225));
	livesText.setOutlineThickness(2.f);
	livesText.setOutlineColor(sf::Color(50, 53, 55));
	livesText.setPosition(sf::Vector2f(40.f, windowSize.y - livesText.getGlobalBounds().size.y * 2.f));

	goldText.setFillColor(sf::Color(220, 223, 225));
	goldText.setOutlineThickness(2.f);
	goldText.setOutlineColor(sf::Color(50, 53, 55));
	goldText.setPosition(sf::Vector2f(40.f, windowSize.y - goldText.getGlobalBounds().size.y * 3.75f));

	waveText.setFillColor(sf::Color(220, 223, 225));
	waveText.setOutlineThickness(2.f);
	waveText.setOutlineColor(sf::Color(50, 53, 55));
	waveText.setPosition(sf::Vector2f(40.f, windowSize.y - waveText.getGlobalBounds().size.y * 5.5f));

	infoText.setFillColor(sf::Color(220, 223, 225));
	//infoText.setOutlineThickness(2.f);
	//infoText.setOutlineColor(sf::Color(50, 53, 55));
	infoText.setPosition(sf::Vector2f(windowSize.x - infoText.getGlobalBounds().size.x - 40.f, windowSize.y - infoText.getGlobalBounds().size.y * 1.6f));
}

void HUD::update(float fixedTimeStep, int lives, int gold, int wave)
{
	if (this->lives != lives)
	{
		this->lives = lives;
		livesText.setString("Lives: " + std::to_string(lives));
	}
	if (this->gold != gold)
	{
		this->gold = gold;
		goldText.setString("Gold: " + std::to_string(gold));
	}
	if (this->wave != wave)
	{
		this->wave = wave;
		waveText.setString("Wave: " + std::to_string(wave));
	}
}

void HUD::render(float interpolationFactor, sf::RenderWindow& window)
{
	window.draw(livesText);
	window.draw(goldText);
	window.draw(waveText);
	window.draw(infoText);
}