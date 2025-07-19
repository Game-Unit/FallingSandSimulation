#include "raylib.h"
#include "Ball.h"
#include <random>
#include <vector>

#define WINDOW_WIDTH (1000)
#define WINDOW_HIEGHT (1000)

int main()
{
    int amountOfBalls = 100;
    std::vector<Ball*> balls(amountOfBalls);

    for (int i = 0; i < amountOfBalls; i++)
    {
        int randomHeightOnScreen = GetRandomValue(0, WINDOW_HIEGHT);
        int randomWidthtOnScreen = GetRandomValue(0, WINDOW_WIDTH);
        balls[i] = new Ball(randomWidthtOnScreen, randomHeightOnScreen, 10, 10, 0.47f, 0.62f);
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HIEGHT, "Physics Engine");

    SetTargetFPS(120);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        for (Ball* ball : balls)
        {
            //ball->AddForce(Vector2{5, 0});
            ball->Update();

            for (int i = 0; i < amountOfBalls; i++)
            {
                if (balls[i] == ball) continue;
                
                ball->Collide(balls[i]);
            }
        }

        // ball1->Update();
        // ball1->AddForce(Vector2{-5, 0});
        // ball1->Collide(ball2);

        // ball2->Update();
        // ball2->AddForce(Vector2{5, 0});

        EndDrawing();
    }
    for (Ball* ball : balls) {
        // need to remove the from the memory
        delete ball;
    }

    CloseWindow();
}