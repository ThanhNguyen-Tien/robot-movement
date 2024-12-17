#pragma once

#include "grid.hpp"
#include <fstream>
#include <sstream>

#define _USE_MATH_DEFINES 1
#include <math.h>

constexpr int TABLE_SIZE = 360;

// Base class for all commands
class Command 
{
public:
    virtual void execute_() = 0;
	virtual ~Command() = default;
};

// Dimension command
class DimensionCommand : public Command 
{
public:
    DimensionCommand(int size) : dimension_(size) {}

private:
	void execute_() override 
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
    void execute_() override {
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
    void execute_() override 
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

        // error represents the balance shift between the x and y axis
        // moving follow x-axis -> error decrease
        // moving follow y-axis -> error increase
        int error = (deltaX > deltaY ? deltaX : -deltaY) / 2;

        while (true) 
        {
            grid.mark(x0, y0, '+');

            if (x0 >= targetX_ && y0 >= targetY_) break;
            //TODO: Handle case x0 > targetX_ || y0 > targetY_

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

// CircleTo Command
class CircleToCommand : public Command {
public:
    CircleToCommand(int radius) : radius_(radius) {
        if (sin_table_.empty() || cos_table_.empty()) {
            sin_table_ = generateSinTable_();
            cos_table_ = generateCosTable_();
        }
    }

    void execute_() override {
        Grid& grid = Grid::instance(); // Lấy instance của Grid
        auto center = grid.getCurrentPosition(); // Lấy vị trí tâm hình tròn

        int xc = center.x;
        int yc = center.y;

        for (int angle = 0; angle < TABLE_SIZE; ++angle) {
            int x = static_cast<int>(round(xc + cos_table_[angle]));
            int y = static_cast<int>(round(yc + sin_table_[angle]));

            grid.mark(x, y, '+');
        }
    }

private:
    int radius_;
    std::vector<int> sin_table_; // Lookup Table
    std::vector<int> cos_table_; // Lookup Table

    std::vector<int> generateSinTable_() {
        std::vector<int> sinTable(TABLE_SIZE);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            double angle = 2 * M_PI * i / (double)TABLE_SIZE;
            sinTable[i] = static_cast<int>(round(radius_ * sin(angle)));
        }
        return sinTable;
    }

    std::vector<int> generateCosTable_() {
        std::vector<int> cosTable(TABLE_SIZE);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            double angle = 2 * M_PI * i / (double)TABLE_SIZE;
            cosTable[i] = static_cast<int>(round(radius_ * cos(angle)));
        }
        return cosTable;
    }
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
        else if (command == "CIRCLE_TO")
        {
            int r;
            stream >> r;
            commands.push_back(new CircleToCommand(r));
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