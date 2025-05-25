#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_BALLS 10

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
    bool is_draggin;
} Ball;


typedef struct {
    Ball balls[MAX_BALLS];
    int count;
    int selected_ball;
    Vector2 mouse_last;
    float drag_last_time;
} GameState;

static GameState game_state;

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

    Color colors[8] = {BLUE, RED, YELLOW, BROWN, BEIGE, PURPLE, PINK, GREEN};
    game_state.count = 10;
    game_state.selected_ball = -1;
    for (int i=0;i<game_state.count;i++){
        int random = GetRandomValue(0, 6);
        game_state.balls[i].is_draggin = false;
        game_state.balls[i].color = colors[random];
        game_state.balls[i].position = (Vector2){300 + i*50, 300};
        game_state.balls[i].velocity = (Vector2){ 0 };

        game_state.balls[i].radius = 20;
    }
}

void UpdateGame(void)
{
    float dt = GetFrameTime();
    Vector2 mouse = GetMousePosition();
    for (int i=0;i<game_state.count;i++){
        Ball* ball = &game_state.balls[i];
        
        // Mouse press inside the ball
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsMouseInsideBall(mouse, *ball)) {
            ball->is_draggin = true;
            ball->velocity = (Vector2){ 0 };
            game_state.drag_last_time = GetTime();
            game_state.mouse_last = mouse;
        }

        // Mouse drag
        if (ball->is_draggin && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            ball->position = mouse;
        }

        // Mouse release
        if (ball->is_draggin && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            float now = GetTime();
            float elapsed = now - game_state.drag_last_time;

            if (elapsed > 0) {
                ball->velocity.x = (mouse.x - game_state.mouse_last.x) / elapsed;
                ball->velocity.y = (mouse.y - game_state.mouse_last.y) / elapsed;
            }

            ball->is_draggin = false;
        }

        if (!ball->is_draggin)
        {
            // Apply gravity
            ball->velocity.y += gravity * dt;

            // Apply user input forces
            if (IsKeyDown(KEY_RIGHT)) ball->velocity.x += forceAmount * dt;
            if (IsKeyDown(KEY_LEFT))  ball->velocity.x -= forceAmount * dt;
            if (IsKeyDown(KEY_UP))    ball->velocity.y -= forceAmount * dt;
            if (IsKeyDown(KEY_DOWN))  ball->velocity.y += forceAmount * dt;

            // Update position
            ball->position.x += ball->velocity.x * dt;
            ball->position.y += ball->velocity.y * dt;

            // Collision with window edges
            if (ball->position.x - ball->radius <= 0) {
                ball->position.x = ball->radius;
                ball->velocity.x *= -restitution;
            }
            if (ball->position.x + ball->radius >= screenWidth) {
                ball->position.x = screenWidth - ball->radius;
                ball->velocity.x *= -restitution;
            }
            if (ball->position.y - ball->radius <= 0) {
                ball->position.y = ball->radius;
                ball->velocity.y *= -restitution;
            }
            if (ball->position.y + ball->radius >= screenHeight) {
                ball->position.y = screenHeight - ball->radius;
                ball->velocity.y *= -restitution;
            }

            // Damping
            ball->velocity.x *= damping;
            ball->velocity.y *= damping;
        }

    }

}


void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    Draw2DGrid();
    for (int i = 0; i < game_state.count; i++) {
        Ball* ball = &game_state.balls[i];
        DrawCircleV(ball->position, ball->radius, ball->color);
    }

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

