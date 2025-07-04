#include "raylib.h"
#include "Button.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

struct Cell
{
    int x, y;
    int value = 0;
};

enum DrawType
{
    sand,
    hardObject,
    erase
};

int main()
{
    float accumulator = 0.0f;
    const float fixedTimeStep = 1.0f / 240.0f;

    // window Dimentions
    const int windowWidth = 512;
    const int windowHeight = 720;

    // grid
    const int gridSize = 2;
    const int columns = windowWidth / gridSize;
    const int rows = windowHeight / gridSize;
    Cell grid[columns][rows];

    std::random_device rd;
    std::mt19937 rng(rd());

    // build shufflable column order
    std::vector<int> columnOrder(columns);
    std::iota(columnOrder.begin(), columnOrder.end(), 0);

    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Falling Sand Simulation");

    Button startButton = {"Graphics/Start Button.png", {windowWidth/2 - 75, 20}, 1};
    Button pauseButton = {"Graphics/pause.png", {460, 20}, 0.3};
    Button sandButton = {"Graphics/RedColorCircal.png", {460, 100}, 0.3f};
    Button HardObjectButton = {"Graphics/BlackColorCircal.png", {460, 150}, 0.3f};
    Button eraserButton = {"Graphics/Eraser.png", {460, 200}, 0.3f};

    Button plusButton = {"Graphics/plus.png", {480, 250}, 0.15f};
    Button minusButton = {"Graphics/minus.png", {450, 250}, 0.15f};
    DrawType drawType = sand;

    int brushSize = 5;

    bool SimulationStarted = false;
    bool buttonPressed = false;

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();
        
        ClearBackground(WHITE);

        if (SimulationStarted)
        {
            accumulator += GetFrameTime();
            while (accumulator >= fixedTimeStep)
            {
                std::shuffle(columnOrder.begin(), columnOrder.end(), rng);
                bool updated[columns][rows]{false};
                for (int xi = 0; xi < columns; xi++)
                {
                    int x = columnOrder[xi];
                    for (int y = rows - 1; y >= 0; --y)
                    {
                        if (grid[x][y].value == 1 && !updated[x][y])
                        {
                            int newX = x;
                            int newY = y;

                            // Try to move down
                            if (y + 1 < rows && grid[x][y + 1].value == 0)
                            {
                                newY = y + 1;
                            }
                            else if (y + 1 < rows
                            && grid[x][y + 1].value != 0
                            && !updated[x][y + 1])  
                            {
                                // Optionally slide left/right
                                
                                //printf(TextFormat("%i", StillFalling(x, y)));
                                bool canLeft  = (x - 1 >= 0) && (grid[x - 1][y + 1].value == 0) && (grid[x - 1][y].value != 2);
                                bool canRight = (x + 1 < columns) && (grid[x + 1][y + 1].value == 0) && (grid[x + 1][y].value != 2);
                                    
                                if (canLeft || canRight)
                                {
                                    int side = 0;
                                    if (canLeft && canRight)
                                        side = (rand() % 2) ? -1 : 1;
                                    else
                                        side = canLeft ? -1 : 1;

                                    newX = x + side;
                                    newY = y + 1;
                                }
                            }

                            if (newX != x || newY != y)
                            {
                                grid[x][y].value = 0;
                                grid[newX][newY].value = 1;
                                updated[x][y] = true;
                                updated[newX][newY] = true;
                            }
                        }
                    }
                }
                accumulator -= fixedTimeStep;
            }
        }

        int centerX = int(mousePosition.x) / gridSize;
        int centerY = int(mousePosition.y) / gridSize;
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (startButton.isPressed(mousePosition, true) && !SimulationStarted)
            {
                SimulationStarted = true;
                buttonPressed = true;
            }
            else if (sandButton.isPressed(mousePosition, true))
            {
                drawType = sand;
                buttonPressed = true;
            }
            else if (HardObjectButton.isPressed(mousePosition, true))
            {
                drawType = hardObject;
                buttonPressed = true;
            }
            else if (eraserButton.isPressed(mousePosition, true))
            {
                drawType = erase;
                buttonPressed = true;
            }
            else if (plusButton.isPressed(mousePosition, true))
            {
                brushSize++;
                if (brushSize > 100)
                    brushSize = 100;
                buttonPressed = true;
            }
            else if (minusButton.isPressed(mousePosition, true))
            {
                brushSize--;
                if (brushSize < 1)
                    brushSize = 1;
                buttonPressed = true;
            }
            else if (pauseButton.isPressed(mousePosition, true) && SimulationStarted)
            {
                SimulationStarted = false;
            }
            else
            {
                buttonPressed = false;
            }
        }

        int radiusPx = brushSize / 2;
        int radiusCells = (radiusPx + gridSize/2) / gridSize;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (!buttonPressed)
            {
                for (int dx = -radiusCells; dx <= radiusCells; ++dx) 
                {
                    for (int dy = -radiusCells; dy <= radiusCells; ++dy) 
                    {
                        int x = centerX + dx;
                        int y = centerY + dy;
                        if (x < 0 || x >= columns || y < 0 || y >= rows) continue;

                        // Compute pixel offset from center
                        int px = dx * gridSize;
                        int py = dy * gridSize;

                        // Check if (px,py) is within the circle radius
                        if (px*px + py*py <= radiusPx * radiusPx) 
                        {
                            switch (drawType) 
                            {
                                case sand:         
                                    grid[x][y].value = 1; 
                                    break;
                                case hardObject:   
                                    grid[x][y].value = 2; 
                                    break;
                                case erase:        
                                    grid[x][y].value = 0; 
                                    break;
                            }
                        }
                    }
                }  
            }
        }

        for (int x = 0; x < columns; x++)
        {
            for (int y = 0; y < rows; y++)
            {
                grid[x][y].x = x * gridSize;
                grid[x][y].y = y * gridSize;
                
                if (grid[x][y].value == 1)
                    DrawRectangle(grid[x][y].x, grid[x][y].y, gridSize, gridSize, RED);
                else if (grid[x][y].value == 2)
                    DrawRectangle(grid[x][y].x, grid[x][y].y, gridSize, gridSize, BLACK);
                //printf("grid[%d][%d] = %d\n", x, y, grid[x][y]);
            }
        }

        if (!SimulationStarted)
        {
            startButton.Draw();
        }
        else
        {
            pauseButton.Draw();
        }

        DrawText(TextFormat("FPS: %i", GetFPS()), 0, 0, 15, RED);
        sandButton.Draw();
        HardObjectButton.Draw();
        eraserButton.Draw();

        plusButton.Draw();
        minusButton.Draw();
        DrawText(TextFormat("Size: %i", brushSize), 450, 280, 15, GREEN);

        EndDrawing();
    }
}