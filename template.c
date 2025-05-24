#include "raylib.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>


static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UnloadGame(void);
static void UpdateDrawFrame(void);

int main(void)
{
    InitWindow(1200, 1200, "Raylib Falling Squares");
    SetTargetFPS(10);  // Slow down to see falling
    InitGame();

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    UnloadGame();
    CloseWindow();
    return 0;
}

void InitGame(void)
{

}

void UpdateGame(void)
{

}

void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawRectangle(10, 10, 30, 30, PINK);
    EndDrawing();
}

void UnloadGame(void)
{
    // Nothing to unload for now
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

