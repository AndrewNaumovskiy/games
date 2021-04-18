
#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Player
{
    Vector2 position;
    Texture2D texture;
    bool carring;
}Player;

typedef struct Turret
{
    Vector2 position;
    bool canShoot;
    int radius;
    bool shooting;
    int target;
} Turret;

typedef struct Base{
    Vector2 position;
    double health;
} Base;

typedef struct Enemy
{
    Vector2 position;
    double health;
    double speed;
    bool attacking;
} Enemy;



Camera2D camera = { 0 };

Player player = { 0 };

Turret turret = { 0 };

Base base = { 0 };

Enemy enemies[10] = { 0 };

bool end = false;

static void InitGame(void);

static void InitPlayer(void);
static void InitTurret(void);
static void InitBase(void);
static void InitCamera(void);
static void InitEnemies(void);

static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);

static void DrawPlayer(void);
static void DrawTurret(void);
static void DrawBase(void);
static void DrawEnemies(void);

static void DrawShots(void);

const static int screenWidth = 1600;
const static int screenHeight = 900;

int main() 
{
    InitWindow(screenWidth, screenHeight, "Bruv kek");

    InitGame();

    InitCamera();

    SetTargetFPS(60); 

    while (!WindowShouldClose()) 
    {
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}


void InitGame(void)
{ 
    end = false;
    
    InitPlayer();
    InitCamera();
    InitTurret();
    InitBase();
    InitEnemies();
}

void InitPlayer(void)
{
    player.position = (Vector2){screenWidth/2, screenHeight / 2};
    player.carring = false;
    player.texture = LoadTexture("graphics/oleg-l.png");
}

void InitCamera(void)
{
    camera.target = (Vector2){ player.position.x + 30, player.position.y + 50 };
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void InitTurret(void)
{
    turret.position = (Vector2){200,200};
    turret.canShoot = true;
    turret.radius = 100;
    turret.shooting = false;
}

void InitBase(void)
{
    base.position = (Vector2){400,400};
    base.health = 100;
}

void InitEnemies(void)
{
    enemies[0].position = (Vector2){1000,200};
    enemies[0].health = 100;
    enemies[0].speed = 1;
    enemies[0].attacking = false;
}

void UpdateGame(void)
{
    if(IsKeyPressed(KEY_ENTER))
    {
        InitGame();
    }
    
    if(end)
        return;

    if(base.health < 0)
    {
        end = true;
    }

    int speed = (player.carring) ? 2 : 5;

    if (IsKeyDown(KEY_LEFT)) 
    {
        player.position.x -= speed;
    }
    if (IsKeyDown(KEY_RIGHT)) 
    {
        player.position.x += speed;
    }
    if(IsKeyDown(KEY_UP))
    {
        player.position.y -= speed;
    }
    if(IsKeyDown(KEY_DOWN))
    {
        player.position.y += speed;
    }

    if(player.carring)
    {
        turret.position = (Vector2) {player.position.x, player.position.y - 20};
    }

    
    if(IsKeyPressed(KEY_SPACE) && CheckCollisionRecs( (Rectangle){player.position.x,player.position.y, 60,120},(Rectangle){turret.position.x,turret.position.y, 40,40} ))
    {
        if(player.carring)
        {
            player.carring = false;
            turret.canShoot = true;
        }
        else
        {
            player.carring = true;
            turret.canShoot = false;
        }
    }


    if(turret.canShoot)
    {
        for (int i = 0; i < 10; i++)
        {
            if(CheckCollisionCircles(turret.position, turret.radius, enemies[i].position, 20))
            {
                turret.target = i;
                turret.shooting = true;
                enemies[i].health -= 0.5;
            }
        }
    }
    else
    {
        turret.shooting = false;
    }

    int count = (sizeof enemies / sizeof *enemies);

    for (size_t i = 0; i < count; i++)
    {
        if(enemies[i].health <= 0)
        {
            memmove(&enemies[i], &enemies[i + 1], (count - i - 1) * sizeof * enemies);
            memset(&enemies[count - 1], 0 , sizeof*enemies);
        }
    }

    for (int i = 0; i < count; i++)
    {
        if(enemies[i].attacking)
            continue;
        if(abs(enemies[i].position.x - base.position.x) > abs(enemies[i].position.y - base.position.y))
        {
            if(enemies[i].position.x - base.position.x > 0)
            {
                enemies[i].position.x -= enemies[i].speed;
            }
            else if(base.position.x - enemies[i].position.x > 0)
            {
                enemies[i].position.x += enemies[i].speed;
            }
        }
        else
        {
            if(enemies[i].position.y - base.position.y > 0)
            {
                enemies[i].position.y -= enemies[i].speed;
            }
            
            else if(base.position.y - enemies[i].position.y > 0)
            {
                enemies[i].position.y += enemies[i].speed;
            }
        }
    }
    
    for (int i = 0; i < count; i++)
    {
        if(CheckCollisionCircles(base.position, 40, enemies[i].position, 40))
        {
            enemies[i].attacking = true;
            base.health -= 0.6;
        }
    }

    if(base.health < 100)
    {
        bool shouldHeal = true;
        for (int i = 0; i < count; i++)
        {
            if(enemies[i].attacking)
            {
                shouldHeal = false;
                break;
            }
        }

        if(shouldHeal)
        {
            base.health += 0.05;
        }
    }
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if(end)
    {
        DrawRectangle(camera.target.x - 1000, camera.target.y - 1000, 2000, 2000, YELLOW);
        DrawText("Bruv KEK", camera.target.x - 100, camera.target.y - 10, 60,BLACK);
    }
    else
    {
        BeginMode2D(camera);

        DrawPlayer();

        DrawTurret();

        DrawBase();

        DrawEnemies();

        DrawShots();

        EndMode2D();
    }

    EndDrawing();
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

void DrawPlayer(void)
{
    DrawTexture(player.texture, player.position.x,player.position.y, RAYWHITE);
}

void DrawTurret(void)
{
    DrawRectangle(turret.position.x,turret.position.y, 40,40,GREEN);

    DrawCircleLines(turret.position.x + 20,turret.position.y + 20, turret.radius, GRAY);
}

void DrawBase(void)
{
    DrawRectangle(base.position.x,base.position.y, 40,40,BROWN);
    DrawRectangleLines(base.position.x - 6, base.position.y - 16, 52,12, BLACK);
    float percentHP = base.health / 100;
    DrawRectangle(base.position.x - 5, base.position.y - 15, 50 * percentHP,10, GREEN);
}

void DrawEnemies(void)
{
    for(int i = 0; i < (sizeof enemies / sizeof *enemies); i++)
    {
        DrawRectangle(enemies[i].position.x,enemies[i].position.y, 40,40,RED);
        DrawRectangleLines(enemies[i].position.x - 6, enemies[i].position.y - 16, 52,12, BLACK);
        float percentHP = enemies[i].health / 100;
        DrawRectangle(enemies[i].position.x - 5, enemies[i].position.y - 15, 50 * percentHP,10, GREEN);

        if(enemies[i].attacking)
        {
           DrawLine(enemies[i].position.x + 20, enemies[i].position.y + 20, base.position.x + 20, base.position.y + 20, RED);
        }
    }
}

void DrawShots(void)
{
    if(turret.shooting)
    {
        
        DrawLine(turret.position.x + 20, turret.position.y + 20, enemies[turret.target].position.x + 20, enemies[turret.target].position.y + 20, GREEN);
    }
}