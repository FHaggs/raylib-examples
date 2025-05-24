#include "raylib.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define GRID_WIDTH    20
#define GRID_HEIGHT   30
#define CELL_SIZE     30  // Pixel size of each cell


static int Board[GRID_WIDTH][GRID_HEIGHT];

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
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            Board[i][j] = 0;
        }
    }

    // Place initial square
    Board[5][0] = 1;
    Board[5][1] = 1;
}
void MoveDown(int x, int y){
    Board[x][y + 1] = 1;
    Board[x][y] = 0;
}

void TryMoveSide(int x, int y) {
    bool canMoveLeft = false;
    bool canMoveRight = false;

    // Check if left is inside bounds and empty
    if (x > 0 && y + 1 < GRID_HEIGHT && Board[x - 1][y + 1] == 0) {
        canMoveLeft = true;
    }

    // Check if right is inside bounds and empty
    if (x + 1 < GRID_WIDTH && y + 1 < GRID_HEIGHT && Board[x + 1][y + 1] == 0) {
        canMoveRight = true;
    }

    // Try to move randomly if both are possible
    if (canMoveLeft && canMoveRight) {
        if (GetRandomValue(0, 1) == 0) {
            Board[x - 1][y + 1] = 1;
        } else {
            Board[x + 1][y + 1] = 1;
        }
        Board[x][y] = 0;
    }
    // Move left only
    else if (canMoveLeft) {
        Board[x - 1][y + 1] = 1;
        Board[x][y] = 0;
    }
    // Move right only
    else if (canMoveRight) {
        Board[x + 1][y + 1] = 1;
        Board[x][y] = 0;
    }
    // Else do nothing
}


void UpdateGame(void)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 click = GetMousePosition();
        int cellX = (int)(click.x / CELL_SIZE);
        int cellY = (int)(click.y / CELL_SIZE);
        if (cellX >= 0 && cellX < GRID_WIDTH && cellY >= 0 && cellY < GRID_HEIGHT) {
            // Toggle cell
            Board[cellX][cellY] = 1 - Board[cellX][cellY];
            printf("Toggled cell: (%d, %d)\n", cellX, cellY);
        } else {
            printf("Click outside grid: (%d, %d)\n", cellX, cellY);
        }
    }
    // Let square fall from bottom to top (reverse order)
    for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (Board[x][y]) {
                if(Board[x][y+1] == 0)
                    MoveDown(x, y);
                else {
                    TryMoveSide(x, y);
                }
            }
        }
    }
}

void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            if (Board[i][j] == 1) {
                DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, PINK);
            }
            // Draw grid lines (optional for clarity)
            DrawRectangleLines(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
        }
    }

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

