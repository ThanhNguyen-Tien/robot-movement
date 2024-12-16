#pragma once

#include "grid.hpp"
// Base class for all commands
class Command {
public:
	virtual void execute() = 0;
	virtual ~Command() = default;
};

// DIMENSION command
class DimensionCommand : public Command {

public:
	DimensionCommand(int dim) : dimension_(dim) {}

	void execute() override {
		Grid::instance().setDimension(dimension_);
	}

private:
	int dimension_;
};

// MOVE_TO command
//class MoveToCommand : public Command {
//
//public:
//	MoveToCommand() = default;
//
//	void execute() override {
//		Grid::instance().setCurrentPosition(x, y);
//	}
//};