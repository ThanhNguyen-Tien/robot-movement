#include <iostream>
#include "grid.hpp"
#include "command.hpp"

int main() {
	// File Path
	const std::string filename = "commandsForTesting.txt";

	// Storing all commands to a vector
	std::vector<Command*> commands = readCommandsFromFile(filename);

	//Handling all Commands
	for (const auto& command : commands) 
	{
		command->execute();
	}

	// Free Memory
	cleanupCommands(commands);

	// Draw the grid
	std::cout << "Initial grid:\n";
	Grid::instance().draw();

	return 0;
}
