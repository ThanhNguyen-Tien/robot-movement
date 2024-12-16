#include <iostream>
#include "grid.hpp"
#include "command.hpp"

int main() {
    // Input grid size (DIMENSION command)
    int n;
    std::cout << "Enter grid dimension (N): ";
    std::cin >> n;

    if (n <= 0) {
        std::cerr << "Error: Dimension must be greater than 0.\n";
        return 1;
    }

    // Execute DIMENSION command
    DimensionCommand dimCmd(n);
    dimCmd.execute();

    Grid::instance().mark(1, 1, '+');

    // Draw the initial grid
    std::cout << "Initial grid:\n";
    Grid::instance().draw();

    return 0;
}
