#include "Grid.hpp"
// ================================================================================================
// File: Grid.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines the Grid class, which contains a collection of Tile objects representing 
//              the game grid. It is responsible for initializing the grid and rendering it to the 
//              window.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Utility.hpp"

Grid::Grid(int rows, int cols, float tileSize) :
	rows(rows),
	cols(cols),
	tileSize(tileSize)
{
	generateNewRandomLevel(rows, cols, tileSize);

	/*for (int i = 0; i < rows; ++i)
	{
		std::vector<Tile> row;
		for (int j = 0; j < cols; ++j)
		{
			row.emplace_back(j, i, tileSize);
		}
		tiles.push_back(row);
	}*/
}

void Grid::render(float interpolationFactor, sf::RenderWindow& window)
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			tile.render(interpolationFactor, window);
		}
	}
}

void Grid::generateNewRandomLevel(int rows, int cols, float tileSize)
{
	bool doAllRowsHavePathableTiles = false;

	while (!doAllRowsHavePathableTiles)
	{
		// Initialize a grid of tiles with dimensions rows x cols
		std::vector<std::vector<Tile>> level(rows, std::vector<Tile>(cols, Tile(Tile::Type::Unassigned, 0, 0, 0.f)));

		std::vector<bool> rowHasPathableTile(rows, false);
		// Set the first and last rows as pathable tiles because paths will never be generated in the first and last rows
		rowHasPathableTile[0] = true;
		rowHasPathableTile[size_t(rows - 1)] = true;

		int currentRow = Utility::randomNumber(1, rows - 2);
		int currentCol = 0;
		level[currentRow][currentCol] = Tile(Tile::Type::Pathable, currentCol, currentRow, tileSize);

		int previousTileDirection = 0;

		// Fill the grid with pathable tiles randomly from left to right
		// Start from the second column (index 1) to avoid the first column
		for (int col = 1; col < cols; ++col)
		{
			int previousRow = currentRow;
			int previousCol = col - 1;

			// Randomly roll for a new direction
			int roll = Utility::randomNumber(0, 9);
			int verticalSteps = Utility::randomNumber(1, rows);
			int nextTileDirection = 0;

			// Don't move up or down if at the last column
			if (col != cols - 1)
			{
				// Move up (50% chance), but only if last move was not down
				if (roll < 5 && previousTileDirection != 1)
					nextTileDirection = -1;
				// Move down (50% chance), but only if last move was not up
				else if (roll > 4 && previousTileDirection != -1)
					nextTileDirection = 1;
				// Otherwise, stay in the same row (0% chance)
			}

			previousTileDirection = nextTileDirection;

			// Perform the vertical step movement
			for (int i = 0; i < verticalSteps; ++i)
			{
				// Add the direction to the current row to determine the next tile's row.
				// Ensure it stays within the bounds of the grid and doesn't move
				// up if at the second-to-top row or down if at the second-to-bottom row
				currentRow = std::clamp(currentRow + nextTileDirection, 1, rows - 2);
				level[currentRow][col] = Tile(Tile::Type::Pathable3, col, currentRow, tileSize);
				rowHasPathableTile[currentRow] = true;
			}

			// Ensure orthogonal connection if the next tile is not in the same row
			if (currentRow != previousRow)
			{
				int step = (currentRow > previousRow) ? 1 : -1;
				for (int row = previousRow; row != currentRow; row += step)
				{
					level[row][col] = Tile(Tile::Type::Pathable2, col, row, tileSize);
					rowHasPathableTile[currentRow] = true;
				}
			}

			// Fill the current tile with a pathable tile
			level[currentRow][col] = Tile(Tile::Type::Pathable, col, currentRow, tileSize);
			rowHasPathableTile[currentRow] = true;
		}

		// Check if all rows have at least one pathable tile
		if (std::all_of(rowHasPathableTile.begin(), rowHasPathableTile.end(), [](bool hasPathableTile) { return hasPathableTile; }))
			doAllRowsHavePathableTiles = true;

		// Fill the rest of the grid with buildable tiles
		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < cols; ++col)
			{
				if (level[row][col].getType() == Tile::Type::Unassigned)
				{
					level[row][col] = Tile(Tile::Type::Buildable, col, row, tileSize);
				}
			}
		}

		tiles = std::move(level);
	}
}