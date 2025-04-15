## Conway's Game of Life - Continuous Automata

This project implements a variation of Conway's Game of Life using continuous automata. The animation is built using the Empirical library's web utilities and Animate class.

### Features
- **Toroidal Grid**: The grid wraps around at the edges, creating a seamless simulation.
- **Glider Initialization**: Randomly populates the grid with glider patterns.
- **Dynamic Cell States**: Cells have continuous states, allowing for smooth transitions.
- **Neighbor Analysis**: Calculates average states of near and distant neighbors to determine cell behavior.

### How It Works
1. **Initialization**: The grid is populated with a percentage of live cells in glider patterns.
2. **State Updates**: Each cell's state is updated based on the average states of its neighbors.
3. **Animation**: The grid is redrawn on each frame, showing the evolution of the automaton.

### Controls
- **Toggle**: Start or stop the animation.
- **Step**: Advance the simulation by one generation.

### Dependencies
- [Empirical Library](https://github.com/devosoft/Empirical)

### Usage
To run the simulation, compile and execute the `CAAAnimate.cpp` file. The animation will be displayed in a web browser.

### Author
Jared Arroyo Ruiz

### Acknowledgments
This project was inspired by Anya E. Vostinar's Neighbor Finder and Gradients Lab. It builds upon a foundational understanding of these labs and was developed as part of Anya's Digital Evolution and Artificial Life Cellular Automaton Homework Assignment in Spring 2025.

Additionally, tools like GitHub Copilot and Gemini were utilized to assist in writing comments and formatting this README file, ensuring clarity and consistency throughout the documentation.
