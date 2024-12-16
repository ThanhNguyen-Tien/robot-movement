#pragma once

#include "grid.hpp"
// Base class for all commands
class Command {
public:
	virtual ~Command() = default;
private:
    virtual void execute_() = 0;
};

// Dimension command
class DimensionCommand : public Command {

public:
    DimensionCommand()
    {
        dimension_ = 0;
    }

    static DimensionCommand& instance()
    {
        static DimensionCommand dimensionCommand_;
        return dimensionCommand_;
    }

    void createGrid(uint16_t size)
    {
        dimension_ = size;
        execute_();
    }

private:
	void execute_() override {
		Grid::instance().setDimension(dimension_);
	}

private:
    uint16_t dimension_;
};

// MoveTo Command
class MoveCommand : public Command {
public:
    MoveCommand() 
    {
        targetX_ = 0;
        targetY_ = 0;
    };

    static MoveCommand& instance()
    {
        static MoveCommand moveCommand_;
        return moveCommand_;
    }

    void moveCurrsorTo(int x, int y)
    {
        if (x < 0 || x >= Grid::instance().getSize() ||
            y < 0 || y >= Grid::instance().getSize()) {
            std::cerr << "Error: Target position (" << targetX_ << ", " << targetY_
                << ") is out of bounds!\n";
            return;
        }

        targetX_ = x;
        targetY_ = y;
        execute_();
    }

private:
    void execute_() override {
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
    LineToCommand()
    {
        targetX_ = 0;
        targetY_ = 0;
    };

    static LineToCommand& instance()
    {
        static LineToCommand lineToCommand_;
        return lineToCommand_;
    }

    void drawLineToPoint(int x, int y)
    {
        if (x < 0 || x >= Grid::instance().getSize() ||
            y < 0 || y >= Grid::instance().getSize()) {
            std::cerr << "Error: Target position (" << targetX_ << ", " << targetY_
                << ") is out of bounds!\n";
            return;
        }

        targetX_ = x;
        targetY_ = y;
        execute_();
    }

private:
    void execute_() override {
        auto currentPos = Grid::instance().getCurrentPosition();

        int x0 = currentPos.x;
        int y0 = currentPos.y;

        int deltaX = abs(targetX_ - x0);
        int deltaY = abs(targetY_ - y0);
        int stepX = (targetX_ > x0) ? 1 : -1;
        int stepY = (targetY_ > y0) ? 1 : -1;

        int error = (deltaX > deltaY ? deltaX : -deltaY) / 2;

        while (true) {
            Grid::instance().mark(x0, y0, '+');

            if (x0 == targetX_ && y0 == targetY_) break;

            int error2 = error;

            if (error2 > -deltaX) {
                error -= deltaY;
                x0 += stepX;
            }

            if (error2 < deltaY) {
                error += deltaX;
                y0 += stepY;
            }
        }

        Grid::instance().setCurrentPosition(targetX_, targetY_);
    }

private:
    int targetX_;
    int targetY_;
};