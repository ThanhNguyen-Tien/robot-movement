#pragma once

#include <iostream>
#include <vector>

// Class representing the grid
class Grid
{
public:
	Grid() : size_(0), currentPosition_{ 0,0 } {}

	typedef struct Cell
	{
		char symbol;
		bool isMarked;
	} cell_t;

	typedef struct Position
	{
		int x;
		int y;
	} pos_t;

	static Grid& instance()
	{
		static Grid grid_;
		return grid_;
	}

	void setDimension(int n)
	{
		size_ = n;
		grid_.assign(size_, std::vector<cell_t>(size_, { ' ', false }));
	}

	void draw() const
	{
		for (const auto& row : grid_) 
		{
			for (cell_t cell : row) 
			{
				std::cout << '|' << cell.symbol;
			}
			std::cout << '|' << '\n';
		}
	}

	void mark(int x, int y, char symbol) 
	{
		if (x >= 0 && x < size_ && y >= 0 && y < size_ && !grid_[x][y].isMarked)
		{
			grid_[x][y].symbol = symbol;
			grid_[x][y].isMarked = true;
		}
	}

	int getSize() const 
	{
		return size_;
	}

	Position getCurrentPosition() const 
	{
		return currentPosition_;
	}

	void setCurrentPosition(int x, int y) 
	{
		currentPosition_ = { x, y };
	}

	bool isCellMarked(int x, int y) const 
	{
		if (x >= 0 && x < size_ && y >= 0 && y < size_) 
		{
			return grid_[x][y].isMarked;
		}
		return false;
	}

private:
	std::vector<std::vector<cell_t>> grid_;
	pos_t currentPosition_;
	int size_;
};