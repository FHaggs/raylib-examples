#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Constants
static const int screenWidth  = 1200;
static const int screenHeight = 1200;
static const int gridSize     = 60;

// Physics constants
static const float gravity = 980.0f;      // pixels/s^2
static const float forceAmount = 1200.0f;  // pixels/s^2
static const float damping = 0.99f;       // velocity retention per frame
static const float restitution = 0.8f;    // bounciness (0–1)

// Data structures
typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
} Ball;

// Global state
static Ball ball;
static bool isDragging = false;
static Vector2 mouseLast = { 0 };
static float dragLastTime = 0.0f;


// Function declarations
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UnloadGame(void);
static void UpdateDrawFrame(void);
static void Draw2DGrid(void);

static bool IsMouseInsideBall(Vector2 mouse, Ball b)
{
    return CheckCollisionPointCircle(mouse, b.position, b.radius);
}


int main(void)
{
    InitWindow(screenWidth, screenHeight, "Raylib Physics: Bouncing Ball on Grid");
    SetTargetFPS(60);
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
    ball.position = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    ball.velocity = (Vector2){ 0.0f, 0.0f };
    ball.radius = 20.0f;
    ball.color = BLUE;
}

void UpdateGame(void)
{
    float dt = GetFrameTime();
    Vector2 mouse = GetMousePosition();

    // Mouse press inside the ball
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsMouseInsideBall(mouse, ball)) {
        isDragging = true;
        ball.velocity = (Vector2){ 0 };
        dragLastTime = GetTime();
        mouseLast = mouse;
    }

    // Mouse drag
    if (isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        ball.position = mouse;
    }

    // Mouse release
    if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        float now = GetTime();
        float elapsed = now - dragLastTime;

        if (elapsed > 0) {
            ball.velocity.x = (mouse.x - mouseLast.x) / elapsed;
            ball.velocity.y = (mouse.y - mouseLast.y) / elapsed;
        }

        isDragging = false;
    }

    if (!isDragging)
    {
        // Apply gravity
        ball.velocity.y += gravity * dt;

        // Apply user input forces
        if (IsKeyDown(KEY_RIGHT)) ball.velocity.x += forceAmount * dt;
        if (IsKeyDown(KEY_LEFT))  ball.velocity.x -= forceAmount * dt;
        if (IsKeyDown(KEY_UP))    ball.velocity.y -= forceAmount * dt;
        if (IsKeyDown(KEY_DOWN))  ball.velocity.y += forceAmount * dt;

        // Update position
        ball.position.x += ball.velocity.x * dt;
        ball.position.y += ball.velocity.y * dt;

        // Collision with window edges
        if (ball.position.x - ball.radius <= 0) {
            ball.position.x = ball.radius;
            ball.velocity.x *= -restitution;
        }
        if (ball.position.x + ball.radius >= screenWidth) {
            ball.position.x = screenWidth - ball.radius;
            ball.velocity.x *= -restitution;
        }
        if (ball.position.y - ball.radius <= 0) {
            ball.position.y = ball.radius;
            ball.velocity.y *= -restitution;
        }
        if (ball.position.y + ball.radius >= screenHeight) {
            ball.position.y = screenHeight - ball.radius;
            ball.velocity.y *= -restitution;
        }

        // Damping
        ball.velocity.x *= damping;
        ball.velocity.y *= damping;
    }
}


void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    Draw2DGrid();
    DrawCircleV(ball.position, ball.radius, ball.color);

    EndDrawing();
}

void UnloadGame(void)
{
    // Nothing to unload yet
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

void Draw2DGrid(void)
{
    for (int x = 0; x < screenWidth; x += gridSize)
        DrawLine(x, 0, x, screenHeight, LIGHTGRAY);

    for (int y = 0; y < screenHeight; y += gridSize)
        DrawLine(0, y, screenWidth, y, LIGHTGRAY);
}

