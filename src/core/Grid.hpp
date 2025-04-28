// ================================================================================================
// File: Grid.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines the Grid class, which contains a collection of Tile objects representing 
//              the game grid. It is responsible for initializing the grid and rendering it to the 
//              window.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <vector>
#include "Tile.hpp"

class Grid
{
public:
	Grid(int rows, int cols, float tileSize);

	void render(float interpolationFactor, sf::RenderWindow& window);

	void generateNewRandomLevel(int rows, int cols, float tileSize);

private:
	int rows, cols;
	float tileSize;
	std::vector<std::vector<Tile>> tiles;
};