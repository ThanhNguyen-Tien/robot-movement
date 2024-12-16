#pragma once

#include "grid.hpp"
#include <fstream>
#include <sstream>

// Base class for all commands
class Command 
{
public:
    virtual void execute() = 0;
	virtual ~Command() = default;
};

// Dimension command
class DimensionCommand : public Command 
{
public:
    DimensionCommand(int size) : dimension_(size) {}

private:
	void execute() override 
    {
        dimension_ = dimension_;
		Grid::instance().setDimension(dimension_);
	}

private:
    uint16_t dimension_;
};

// MoveTo Command
class MoveToCommand : public Command 
{
public:
    MoveToCommand(int x, int y) : targetX_(x), targetY_(y) {}

private:
    void execute() override {
        if (targetX_ < 0 || targetX_ >= Grid::instance().getSize() ||
            targetY_ < 0 || targetY_ >= Grid::instance().getSize()) {
            std::cerr << "Error: Target position (" << targetX_ << ", " << targetY_
                << ") is out of bounds!\n";
            return;
        }

        Grid::instance().setCurrentPosition(targetX_, targetY_);
        std::cout << "Moved cursor to (" << targetX_ << ", " << targetY_ << ")\n";
    }

private:
    int targetX_;
    int targetY_;
};

// LineTo Command
class LineToCommand : public Command {
public:
    LineToCommand(int x, int y) : targetX_(x), targetY_(y) {}

private:
    void execute() override 
    {
        Grid& grid = Grid::instance();

        if (targetX_ < 0 || targetX_ >= grid.getSize() ||
            targetY_ < 0 || targetY_ >= grid.getSize()) 
        {
            std::cerr << "Error: Target position (" << targetX_ << ", " << targetY_
                << ") is out of bounds!\n";
            return;
        }
        auto currentPos = grid.getCurrentPosition();

        int x0 = currentPos.x;
        int y0 = currentPos.y;

        int deltaX = abs(targetX_ - x0);
        int deltaY = abs(targetY_ - y0);
        int stepX = (targetX_ > x0) ? 1 : -1;
        int stepY = (targetY_ > y0) ? 1 : -1;

        int error = (deltaX > deltaY ? deltaX : -deltaY) / 2;

        while (x0 < targetX_ && y0 == targetY_)
        {
            if (!grid.isCellMarked(x0, y0))
            {
                grid.mark(x0, y0, '+');
            }

            if () break;

            int error2 = error;

            if (error2 > -deltaX) 
            {
                error -= deltaY;
                x0 += stepX;
            }

            if (error2 < deltaY) 
            {
                error += deltaX;
                y0 += stepY;
            }
        }

        grid.setCurrentPosition(targetX_, targetY_);
    }

private:
    int targetX_;
    int targetY_;
};

std::vector<Command*> readCommandsFromFile(const std::string& filename) 
{
    std::ifstream file(filename);
    std::vector<Command*> commands;
    std::string line;

    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file!" << std::endl;
        return commands;
    }

    while (std::getline(file, line)) 
    {
        std::istringstream stream(line);
        std::string command;
        stream >> command;

        if (command == "DIMENSION") 
        {
            int size;
            stream >> size;
            commands.push_back(new DimensionCommand(size));
        }
        else if (command == "MOVE_TO") 
        {
            int x, y;
            stream >> x >> y;
            commands.push_back(new MoveToCommand(x, y));
        }
        else if (command == "LINE_TO") 
        {
            int x, y;
            stream >> x >> y;
            commands.push_back(new LineToCommand(x, y));
        }
        else 
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }

    return commands;
}

void cleanupCommands(std::vector<Command*>& commands) 
{
    for (auto command : commands) 
    {
        delete command;
    }
}