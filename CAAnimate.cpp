// File: CAAAnimate.cpp
// Created on: April 16th, 2025
// Author: Jared Arroyo Ruiz

// This code is a simple implementation of an Conway's game of life
// using a population of continuous automata.

// Several parts of the code was created following Anya E. Vostinar's 
// Neighbor Finder and Gradients Lab.


// Setting up Environment
#include "emp/web/Animate.hpp" // Include the Animate class for animation functionality
#include "emp/web/web.hpp"     // Include web utilities for creating web-based interfaces
#include "emp/math/Random.hpp" // Include random number generation utilities

emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate {

    // Define constants for the size of each cell and the grid dimensions
    const int cellSize = 15; // Size of each cell in pixels
    const int num_h_boxes = 100; // Number of cells in the grid's height
    const int num_w_boxes = 100; // Number of cells in the grid's width
    const double width{double(num_w_boxes) * cellSize}; // Total width of the canvas
    const double height{double(num_h_boxes) * cellSize}; // Total height of the canvas

    // Define the number of initial cells to populate (1% of the grid)
    const int startCells = int((num_h_boxes * num_w_boxes) / 100);

    // 2D vector to store the state of each cell in the grid
    std::vector<std::vector<float>> cells;

    // Create a canvas for drawing the grid
    emp::web::Canvas canvas{width, height, "canvas"};

    public:

    CAAnimator() {
        // Add the canvas to the document
        doc << canvas;

        // Add toggle and step buttons to the document
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // Initialize a random number generator with a fixed seed for reproducibility
        emp::Random random_gen(444);

        // Draw the initial grid on the canvas with white cells and black borders
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                canvas.Rect(x * cellSize, y * cellSize, cellSize, cellSize, "white", "black");
            }
        }

        // Resize the cells grid to match the number of boxes
        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));

        // Populate the grid with a specified number of gliders
        for (int r = 0; r < startCells; r++) {

            // Generate random coordinates for the glider
            int randX = random_gen.GetInt(0, num_w_boxes); // Random x-coordinate
            int randY = random_gen.GetInt(0, num_h_boxes); // Random y-coordinate

            // Create a glider at the random position
            MakeGlider(randX, randY);
        }
    }

        /**
         * @brief Creates a "glider" pattern in the cellular automaton grid.
         * 
         * This function modifies the `cells` grid to create a glider pattern starting
         * at the specified (x, y) coordinates.
         * 
         * @param x The x-coordinate of the starting position for the glider.
         * @param y The y-coordinate of the starting position for the glider.
         * 
         * The function ensures that the coordinates wrap around the grid boundaries
         * using modular arithmetic, allowing the glider to be placed seamlessly on a toroidal grid.
         */
        void MakeGlider (int x, int y) {

            // Creating Glider Body
            cells[emp::Mod(x, num_w_boxes)][emp::Mod(y, num_h_boxes)] = 1;
            cells[emp::Mod(x + 1, num_w_boxes)][emp::Mod(y, num_h_boxes)] = 1;
            cells[emp::Mod(x, num_w_boxes)][emp::Mod(y + 1, num_h_boxes)] = 1;
            cells[emp::Mod(x + 1, num_w_boxes)][emp::Mod(y + 1, num_h_boxes)] = 1;

            // Creating Glider Tail
            cells[emp::Mod(x - 1, num_w_boxes)][emp::Mod(y - 1, num_h_boxes)] = 1;
            cells[emp::Mod(x - 2, num_w_boxes)][emp::Mod(y - 2, num_h_boxes)] = 1;
            cells[emp::Mod(x - 3, num_w_boxes)][emp::Mod(y - 3, num_h_boxes)] = 1;

        }

        /**
         * @brief Counts the number of live neighbors around a given cell.
         * 
         * This function calculates the number of live neighbors for a cell
         * located at (x, y) in the grid. It uses modular arithmetic to handle
         * wrapping around the grid boundaries.
         * 
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @return The count of live neighbors.
         */
        float NeighborCount(int x, int y) {

            int neighborCount = 0;
        
            // Iterate through the 3x3 neighborhood around the cell
            for (int i = x - 1; i <= x + 1; i++) {

                for (int j = y - 1; j <= y + 1; j++) {

                    // Skip the cell itself
                    if (i == x && j == y) {

                    continue;
                    
                    }
            
                    // Wrap around the grid boundaries
                    int wrapped_i = emp::Mod(i, num_w_boxes);
                    int wrapped_j = emp::Mod(j, num_h_boxes);

                    // Increment the count if the neighbor is alive
                    if (cells[wrapped_i][wrapped_j] == 1) {

                        neighborCount++;

                    }
                }
            }
        
            return neighborCount;
        }

        /**
         * @brief Calculates the average state of the near neighbors of a cell.
         * 
         * This function computes the average value of the states of the 8
         * neighboring cells in the immediate vicinity of the cell at (x, y).
         * 
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @return The average state of the near neighbors.
         */
        float NearNeighborsAvg(int x, int y) {

            float neighborAvg = 0;
        
            // Iterate through the 3x3 neighborhood around the cell
            for (int i = x - 1; i <= x + 1; i++) {

            for (int j = y - 1; j <= y + 1; j++) {

                // Skip the cell itself
                if (i == x && j == y) {
                continue;
                }
        
                // Wrap around the grid boundaries
                int wrapped_i = emp::Mod(i, num_w_boxes);
                int wrapped_j = emp::Mod(j, num_h_boxes);

                // Add the state of the neighbor to the total
                neighborAvg += cells[wrapped_i][wrapped_j];
            }
            }
        
            // Return the average state of the 8 neighbors
            return neighborAvg / 8;
        }

        /**
         * @brief Calculates the average state of the distant neighbors of a cell.
         * 
         * This function computes the average value of the states of the cells
         * in a larger 7x7 neighborhood (excluding the 3x3 near neighbors) around
         * the cell at (x, y).
         * 
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @return The average state of the distant neighbors.
         */
        float DistantNeighborsAvg(int x, int y) {

            float neighborAvg = 0;
        
            // Iterate through the 7x7 neighborhood around the cell
            for (int i = x - 3; i <= x + 3; i++) {

            for (int j = y - 3; j <= y + 3; j++) {

                // Skip the cell itself
                if (i == x && j == y) {
                continue;
                }
        
                // Wrap around the grid boundaries
                int wrapped_i = emp::Mod(i, num_w_boxes);
                int wrapped_j = emp::Mod(j, num_h_boxes);

                // Add the state of the neighbor to the total
                neighborAvg += cells[wrapped_i][wrapped_j];
            }
            }
        
            // Return the average state of the 48 neighbors
            return neighborAvg / 48;
        }

        /**
         * @brief Draws the current state of the cells on the canvas.
         * 
         * This function iterates through the grid of cells and draws each cell
         * on the canvas. The color of each cell is determined by its state, with
         * a gradient hue based on the cell's value.
         */
        void DrawCells() {

            // Iterate through each cell in the grid
            for (int i = 0; i < num_w_boxes; i++) {

                for (int j = 0; j < num_h_boxes; j++) {

                    // Draw a rectangle for each cell with a color based on its state
                    canvas.Rect(
                    i * cellSize, // X-coordinate of the cell
                    j * cellSize, // Y-coordinate of the cell
                    cellSize,     // Width of the cell
                    cellSize,     // Height of the cell
                    emp::ColorHSV(340.0 * cells[i][j], 1 * cells[i][j], 1 * cells[i][j]), // Fill color
                    "black"       // Border color
                    );

                }
            }
        }

        /**
         * @brief Computes the next generation of the cellular automaton.
         * 
         * This function calculates the state of each cell in the grid for the next
         * generation based on the average states of its neighbors and specific rules
         * for live and dead cells.
         * 
         * @return A 2D vector representing the updated state of the grid.
         */
        std::vector<std::vector<float>> NextGeneration() {

            // Create a new grid to store the next generation of cells
            std::vector<std::vector<float>> newCells;
            newCells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));

            // Iterate through each cell in the grid
            for (int i = 0; i < num_w_boxes; i++) {

                for (int j = 0; j < num_h_boxes; j++) {

                    // Calculate the average state of near and distant neighbors
                    float nearNeighborAvg = NearNeighborsAvg(i, j);
                    float distNeighborAvg = DistantNeighborsAvg(i, j);
                    float allNeigborsAvg = (nearNeighborAvg + distNeighborAvg) / 2;

                    // Rules for live cells
                    if (cells[i][j] == 1) { 

                        
                        if (allNeigborsAvg <= 0.8) {

                            // Stay alive if the average state of neighbors is below a threshold
                            newCells[i][j] = (1 + allNeigborsAvg) / 2;

                        } 

                        else {

                            // Die if the average state of neighbors exceeds the threshold
                            newCells[i][j] = 0;

                        }

                    } 

                    // Rules for dead cells
                    else { 

                        if (allNeigborsAvg >= 0.275) {

                            // Become alive if the average state of neighbors is above a threshold
                            newCells[i][j] = (1 + allNeigborsAvg) / 2;
                        } 

                        else {

                            // Stay dead if the average state of neighbors is below the threshold
                            newCells[i][j] = 0;

                        }
                    }
                }
            }

            // Return the updated grid for the next generation
            return newCells;
        }


        /**
         * @brief Updates the animation frame.
         * 
         * This function is called on each frame of the animation. It clears the canvas,
         * redraws the cells based on their current state, and computes the next generation
         * of the cellular automaton.
         */
        void DoFrame() {

            // Clear the canvas to prepare for the next frame
            canvas.Clear();

            // Draw the current state of the cells on the canvas
            DrawCells();
            
            // Compute the next generation of cells and update the grid
            cells = NextGeneration();

        }
};

// Create an instance of the CAAnimator class to handle the animation
CAAnimator animator;

// The main function is currently empty because the animation is managed
// by the CAAnimator class and the emp::web::Animate framework.
int main() {

    return 0;
}