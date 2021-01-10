
#include "raylib.h"

#include <stdio.h>
#include <string.h>


typedef struct Oleg
{
    Vector2 position;
    Texture2D textureL;
    Texture2D textureR;
    bool look;
} Oleg;

typedef struct Event
{
    Vector2 position;
    char message[100];
    Texture2D texture;
    bool triggered;
    bool drawMe;
} Event;

typedef struct LolRect
{
    Vector2 position;
    Texture2D texture;
} LolRect;

typedef struct NPC
{
    Vector2 position;
    Texture2D textureL;
    Texture2D textureR;
    bool following;
    bool look; // true - right. false - left
} NPC;


static Oleg oleg = {0};
static Event event[11] = {0};
static NPC npcs[2] = {0};
static LolRect lolrects[2] = {0};

Camera2D camera = { 0 };

static void InitGame(void);
static void InitCamera(void);

static void InitLolRect(void);
static void InitNPC(void);
static void InitEvents(void);

static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);

static bool end = false;

const static int screenWidth = 1600;
const static int screenHeight = 900;

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    InitWindow(screenWidth, screenHeight, "Oleg's life");

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

    oleg.position = (Vector2){50, 70};
    oleg.textureR = LoadTexture("graphics/oleg-r.png");
    oleg.textureL = LoadTexture("graphics/oleg-l.png");
    oleg.look = true;

    InitLolRect();
    InitNPC();
    InitEvents();
}

void InitCamera(void)
{
    camera.target = (Vector2){ oleg.position.x + 30, oleg.position.y + 50 };
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void InitLolRect(void)
{
    lolrects[0].position = (Vector2){1200, 10};
    lolrects[0].texture = LoadTexture("graphics/found.png");

    lolrects[1].position = (Vector2){2700, 10};
    lolrects[1].texture = LoadTexture("graphics/found.png");
}

void InitNPC(void)
{
    // 0 - liza
    // 1 - andrew

    npcs[0].position = (Vector2){120,70};
    npcs[0].textureR = LoadTexture("graphics/liza-r.png");
    npcs[0].textureL = LoadTexture("graphics/liza-l.png");
    npcs[0].following = false;
    npcs[0].look = false;

    npcs[1].position = (Vector2){4100,70};
    npcs[1].textureR = LoadTexture("graphics/andrew-r.png");
    npcs[1].textureL = LoadTexture("graphics/andrew-l.png");
    npcs[1].following = false;
}

void InitEvents(void)
{
    // first event
    event[0].position = (Vector2){60,10};
    strcpy(event[0].message,"Liza: Hello, Oleg. Let's go play some impressive game\ncalled League of Legends. Go right");
    event[0].triggered = false;
    event[0].drawMe = false;

    event[1].position = (Vector2){-120, 70};
    strcpy(event[1].message,"Liza: Am i joke to you?");
    event[1].triggered = false;
    event[1].drawMe = false;

    event[2].position = (Vector2){900, 10};
    strcpy(event[2].message,"Liza: Okay, so let's just enjoy");
    event[2].triggered = false;
    event[2].drawMe = false;

    event[3].position = (Vector2){1700, -90};
    event[3].texture = LoadTexture("graphics/defeat.png");
    event[3].triggered = false;
    event[3].drawMe = false;

    event[4].position = (Vector2){2000, 10};
    strcpy(event[4].message,"Liza: Dont tilt. We can win. *crunch... crunch*. MMM CHIPSIKI");
    event[4].triggered = false;
    event[4].drawMe = false;

    event[5].position = (Vector2){3200, -90};
    event[5].texture = LoadTexture("graphics/defeat.png");
    event[5].triggered = false;
    event[5].drawMe = false;

    event[6].position = (Vector2){3600, 10};
    strcpy(event[6].message,"Liza: Actually, i dont care\n about wins. Lets play more");
    event[6].triggered = false;
    event[6].drawMe = false;

    event[7].position = (Vector2){4099, -60};
    strcpy(event[7].message,"Andrew: Stop right there. I fucking hate this game.\nLet's go harvest potatoes.\nLiza: MMMM potatoes");
    event[7].triggered = false;
    event[7].drawMe = false;

    event[8].position = (Vector2){4400, -20};
    strcpy(event[8].message,"Oleg: Yeah, fuck this game, let's play Stardew Valley.\nThis is the most comfy game in entire world");
    event[8].triggered = false;
    event[8].drawMe = false;

    event[9].position = (Vector2){5500, -20};
    strcpy(event[9].message," ");
    event[9].triggered = false;
    event[9].drawMe = false;

    // liza following

    event[10].position = (Vector2){270, 10};
    event[10].triggered = false;
    event[10].drawMe = false;

    event[11].position = (Vector2){4330, 10};
    event[11].triggered = false;
    event[11].drawMe = false;

    //for(int i = 0;i<10;i++)
    //{
    //    event[i].position = (Vector2){screenWidth / 2 + 50 * i, screenHeight / 2};
    //    event[i].triggered = false;
    //    event[i].drawMe = true;
    //
    //    char temp[100] = "kek";
    //
    //    sprintf(temp,"%d", i);
    //
    //    strcpy(event[i].message,temp);
    //}
}

void UpdateGame(void)
{
    if (IsKeyDown(KEY_LEFT)) 
    {
        oleg.position.x -= 5;
        oleg.look = false;
        for(int i = 0;i<2;i++)
        {
            npcs[i].look = false;
            if(npcs[i].following)
            {
                npcs[i].position.x -= 5;
            }
        }
    }
    if (IsKeyDown(KEY_RIGHT)) 
    {
        oleg.position.x += 5;
        oleg.look = true;
        for(int i = 0;i<2;i++)
        {
            npcs[i].look = true;
            if(npcs[i].following)
            {
                npcs[i].position.x += 5;
            }
        }
    }

    if(oleg.position.x < 4600)
        camera.target = (Vector2){ oleg.position.x + 30, oleg.position.y + 50 };

    for(int i = 0;i<10;i++)
    {
        // la fin
        if(CheckCollisionRecs((Rectangle){oleg.position.x - 30, oleg.position.y - 50, 60,100}, (Rectangle){event[9].position.x - 50,event[9].position.y - 50, 100, 400}))
        {
            end = true;
        }

        if(CheckCollisionRecs((Rectangle){oleg.position.x - 30, oleg.position.y - 50, 60,100}, (Rectangle){event[10].position.x - 50,event[10].position.y - 50, 100, 400}))
        {
            npcs[0].following = true;
        }

        if(CheckCollisionRecs((Rectangle){oleg.position.x - 30, oleg.position.y - 50, 60,100}, (Rectangle){event[11].position.x - 50,event[11].position.y - 50, 100, 400}))
        {
            npcs[1].following = true;
        }

        if(CheckCollisionRecs((Rectangle){oleg.position.x - 30, oleg.position.y - 50, 60,100}, (Rectangle){event[i].position.x - 50,event[i].position.y - 50, 100, 400}))
        {
            event[i].triggered = true;
            event[i].drawMe = false;
        }
    }

    if(IsKeyPressed(KEY_ENTER))
    {
        InitGame();
    }
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if(end)
    {
        DrawText("The End", 100,100, 100, BLACK);
        DrawText("Enter to restart", 100,200, 40, GRAY);
    }
    else
    {
        BeginMode2D(camera);

        DrawTexture(LoadTexture("graphics/home0.png"), 5000, -180, WHITE);

        if(oleg.look)
            DrawTexture(oleg.textureR, oleg.position.x,oleg.position.y, WHITE);
        else
            DrawTexture(oleg.textureL, oleg.position.x,oleg.position.y, WHITE);
        
        // npcs
        for(int i = 0;i<2;i++)
        {
            if(npcs[i].look)
                DrawTexture(npcs[i].textureR, npcs[i].position.x,npcs[i].position.y, WHITE);
            else
                DrawTexture(npcs[i].textureL, npcs[i].position.x,npcs[i].position.y, WHITE);  
        }

        for(int i = 0; i< 10;i++)
        {
            if(event[i].drawMe)
                DrawRectangle(event[i].position.x - 50,event[i].position.y - 50, 100, 100, RED);
            
            if(event[i].triggered)
            {
                if(strlen(event[i].message) == 0)
                {
                    DrawTexture(event[i].texture, event[i].position.x, event[i].position.y, WHITE);
                }
                else
                {
                    DrawText(event[i].message, event[i].position.x, event[i].position.y, 20, RED);   
                }
            }
        }

        // lol rect
        for(int i = 0; i < 2;i++)
        {
            DrawTexture(lolrects[i].texture,lolrects[i].position.x, lolrects[i].position.y, WHITE);
        }

        // home
        
        DrawTexture(LoadTexture("graphics/home1.png"), 5290, -180, WHITE);

        EndMode2D();    
    }

    EndDrawing();
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}