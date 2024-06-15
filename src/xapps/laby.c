#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "canvas.h"
#include "window.h"
#include "timer.h"
#include "debug.h"

l_ulong UID          = ULONG_ID('L','A','B','Y');
l_ulong AppVersion   = ULONG_ID(0,0,2,0);
char AppName[]       = "Laby";

#define MAX_ENEMIES 10
#define MAX_BOMBS 10

typedef struct {
    int x;
    int y;
    int active;
} Enemy;

typedef struct {
    int x;
    int y;
    int active;
} Bomb;

Enemy Enemies[MAX_ENEMIES];
Bomb Bombs[MAX_BOMBS];
int WeaponX, WeaponY;
int WeaponActive = 0;
int CurrentEnemyCount = 2; // Start with 2 enemies

_PUBLIC l_int WhereX = 1;
_PUBLIC l_int WhereY = 1;
_PUBLIC PCanvas Laby = 0;
_PUBLIC l_int Level  = 1;

/**
*   Map information
*/
_PUBLIC l_int MapLevels = 0;

#define MAPSIZE 20
l_char MapDAT[MAPSIZE][MAPSIZE];
l_char MapDATLevels[4][MAPSIZE][MAPSIZE] =
{
    {
                // Level 1
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0 },
        { 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 2, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    // Placeholder for Level 2
    {
        // Level 2
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
        { 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 2, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    // Placeholder for Level 3
    {
        // Level 3
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    // Placeholder for Level 4
    {
        // Level 4
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
        { 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0, 1, 0, 0, 2, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
};


void InitEnemies()
{
    int i;
    for (i = 0; i < CurrentEnemyCount; i++)
    {
        Enemies[i].x = rand() % MAPSIZE;
        Enemies[i].y = rand() % MAPSIZE;
        Enemies[i].active = 1;
    }
}

void InitWeaponBox()
{
    WeaponX = 10;
    WeaponY = 10;
}

void InitBombs()
{
    int i;
    for (i = 0; i < MAX_BOMBS; i++)
    {
        Bombs[i].active = 0;
    }
}

void MoveEnemies()
{
    int i;
    for (i = 0; i < CurrentEnemyCount; i++)
    {
        if (Enemies[i].active)
        {
            int direction = rand() % 4;

            switch (direction)
            {
                case 0: // Move up
                    if (Enemies[i].y > 0 && MapDAT[Enemies[i].x][Enemies[i].y - 1] == 1) Enemies[i].y--;
                    break;
                case 1: // Move down
                    if (Enemies[i].y < MAPSIZE - 1 && MapDAT[Enemies[i].x][Enemies[i].y + 1] == 1) Enemies[i].y++;
                    break;
                case 2: // Move left
                    if (Enemies[i].x > 0 && MapDAT[Enemies[i].x - 1][Enemies[i].y] == 1) Enemies[i].x--;
                    break;
                case 3: // Move right
                    if (Enemies[i].x < MAPSIZE - 1 && MapDAT[Enemies[i].x + 1][Enemies[i].y] == 1) Enemies[i].x++;
                    break;
            }
        }
    }
}

void DropBomb()
{
    int i;
    for (i = 0; i < MAX_BOMBS; i++)
    {
        if (!Bombs[i].active)
        {
            Bombs[i].x = WhereX;
            Bombs[i].y = WhereY;
            Bombs[i].active = 1;
            break;
        }
    }
}

void CheckBombs()
{
    int i, j;
    for (i = 0; i < MAX_BOMBS; i++)
    {
        if (Bombs[i].active)
        {
            for (j = 0; j < CurrentEnemyCount; j++)
            {
                if (Enemies[j].active && Bombs[i].x == Enemies[j].x && Bombs[i].y == Enemies[j].y)
                {
                    Enemies[j].active = 0;
                    Bombs[i].active = 0;
                    break;
                }
            }
        }
    }
}

_PUBLIC void LoadMap(l_text szFile)
{
    FILE *f = fopen(szFile, "rt");
    int i = 0;

    if (!f) return;

    while (i < MAPSIZE)
    {
        //fscanf(f, "[%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i][%i]", &MapDAT[i][0], &MapDAT[i][1], &MapDAT[i][2], &MapDAT[i][3], &MapDAT[i][4], &MapDAT[i][5], &MapDAT[i][6],&MapDAT[i][7],&MapDAT[i][8],&MapDAT[i][9],&MapDAT[i][10],&MapDAT[i][11],&MapDAT[i][12],&MapDAT[i][13],&MapDAT[i][14],&MapDAT[i][15],&MapDAT[i][16],&MapDAT[i][17],&MapDAT[i][18],&MapDAT[i][19]);
        i++;
    }

    fclose(f);
}

_PUBLIC void GameDraw(PWidget o, p_bitmap buffer, PRect w)
{
    l_int x = 0, y = 0;

    rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, makecol(0,0,0));

    while ( y < 6 )
    {
        while ( x < 6 )
        {
            l_int vx = WhereX-3+x;
            l_int vy = WhereY-3+y;

            if (vx > 0 && vx < MAPSIZE && vy > 0 && vy < MAPSIZE)
            {
                /**
                *   Object?
                */
                if ( MapDAT[vx][vy] == 0 )
                {
                    rectfill(buffer, o->Absolute.a.x+(x*((o->Absolute.b.x-o->Absolute.a.x)/5)) - (o->Absolute.b.x-o->Absolute.a.x)/5, o->Absolute.a.y+(y*((o->Absolute.b.y-o->Absolute.a.y)/5)) - (o->Absolute.b.y-o->Absolute.a.y)/5, o->Absolute.a.x+(x*((o->Absolute.b.x-o->Absolute.a.x)/5)), o->Absolute.a.y+(y*((o->Absolute.b.y-o->Absolute.a.y)/5)), makecol(0,150,0));
                }
                else if ( MapDAT[vx][vy] == 1)
                {
                    rectfill(buffer, o->Absolute.a.x+(x*((o->Absolute.b.x-o->Absolute.a.x)/5)) - (o->Absolute.b.x-o->Absolute.a.x)/5, o->Absolute.a.y+(y*((o->Absolute.b.y-o->Absolute.a.y)/5)) - (o->Absolute.b.y-o->Absolute.a.y)/5, o->Absolute.a.x+(x*((o->Absolute.b.x-o->Absolute.a.x)/5)), o->Absolute.a.y+(y*((o->Absolute.b.y-o->Absolute.a.y)/5)), makecol(0,128,128));
                }
                else if ( MapDAT[vx][vy] == 2)
                {
                    rectfill(buffer, o->Absolute.a.x+(x*((o->Absolute.b.x-o->Absolute.a.x)/5)) - (o->Absolute.b.x-o->Absolute.a.x)/5, o->Absolute.a.y+(y*((o->Absolute.b.y-o->Absolute.a.y)/5)) - (o->Absolute.b.y-o->Absolute.a.y)/5, o->Absolute.a.x+(x*((o->Absolute.b.x-o->Absolute.a.x)/5)), o->Absolute.a.y+(y*((o->Absolute.b.y-o->Absolute.a.y)/5)), makecol(128,128,128));
                }
            }

            x++;
        }

        y++;
        x = 1;
    }

    /**
    *   Character
    */
    rectfill(buffer, o->Absolute.a.x+(o->Absolute.b.x-o->Absolute.a.x)/2-10, o->Absolute.a.y+(o->Absolute.b.y-o->Absolute.a.y)/2-10, o->Absolute.a.x+(o->Absolute.b.x-o->Absolute.a.x)/2+10, o->Absolute.a.y+(o->Absolute.b.y-o->Absolute.a.y)/2+10, makecol(255,5,5));

    /**
    *   Weapon Box
    */
    if (!WeaponActive)
    {
        rectfill(buffer, o->Absolute.a.x + (WeaponX - WhereX + 3) * ((o->Absolute.b.x - o->Absolute.a.x) / 5) - (o->Absolute.b.x - o->Absolute.a.x) / 5,
                         o->Absolute.a.y + (WeaponY - WhereY + 3) * ((o->Absolute.b.y - o->Absolute.a.y) / 5) - (o->Absolute.b.y - o->Absolute.a.y) / 5,
                         o->Absolute.a.x + (WeaponX - WhereX + 3) * ((o->Absolute.b.x - o->Absolute.a.x) / 5),
                         o->Absolute.a.y + (WeaponY - WhereY + 3) * ((o->Absolute.b.y - o->Absolute.a.y) / 5),
                         makecol(0,255,0));
    }

    /**
    *   Enemies
    */
    for (x = 0; x < CurrentEnemyCount; x++)
    {
        if (Enemies[x].active)
        {
            rectfill(buffer, o->Absolute.a.x + (Enemies[x].x - WhereX + 3) * ((o->Absolute.b.x - o->Absolute.a.x) / 5) - (o->Absolute.b.x - o->Absolute.a.x) / 5,
                             o->Absolute.a.y + (Enemies[x].y - WhereY + 3) * ((o->Absolute.b.y - o->Absolute.a.y) / 5) - (o->Absolute.b.y - o->Absolute.a.y) / 5,
                             o->Absolute.a.x + (Enemies[x].x - WhereX + 3) * ((o->Absolute.b.x - o->Absolute.a.x) / 5),
                             o->Absolute.a.y + (Enemies[x].y - WhereY + 3) * ((o->Absolute.b.y - o->Absolute.a.y) / 5),
                             makecol(0,0,255));
        }
    }

    /**
    *   Bombs
    */
    for (x = 0; x < MAX_BOMBS; x++)
    {
        if (Bombs[x].active)
        {
            rectfill(buffer, o->Absolute.a.x + (Bombs[x].x - WhereX + 3) * ((o->Absolute.b.x - o->Absolute.a.x) / 5) - (o->Absolute.b.x - o->Absolute.a.x) / 5,
                             o->Absolute.a.y + (Bombs[x].y - WhereY + 3) * ((o->Absolute.b.y - o->Absolute.a.y) / 5) - (o->Absolute.b.y - o->Absolute.a.y) / 5,
                             o->Absolute.a.x + (Bombs[x].x - WhereX + 3) * ((o->Absolute.b.x - o->Absolute.a.x) / 5),
                             o->Absolute.a.y + (Bombs[x].y - WhereY + 3) * ((o->Absolute.b.y - o->Absolute.a.y) / 5),
                             makecol(255,165,0));
        }
    }
}

/**
*   Fade effect
*/
l_int Fade = 0;
l_bool StartGame = false;

_PUBLIC void  FadeEffectDraw ( PWidget o, p_bitmap buffer, PRect w )
{
    rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, makecol(Fade, Fade*2, Fade));

    // Draw title and menu
    textout_centre_ex(buffer, font, "Laby: The Dawn of Adventure", (o->Absolute.a.x + o->Absolute.b.x) / 2, (o->Absolute.a.y + o->Absolute.b.y) / 4, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, "A Labyrinth Game", (o->Absolute.a.x + o->Absolute.b.x) / 2, (o->Absolute.a.y + o->Absolute.b.y) / 4 + 20, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, "Press Enter to Start Game", (o->Absolute.a.x + o->Absolute.b.x) / 2, (o->Absolute.a.y + o->Absolute.b.y) / 2, makecol(255, 255, 255), -1);
}

_PUBLIC void  FadeEffect ( void )
{
    WIDGET(Laby)->Draw = &FadeEffectDraw;

    while (Fade < 100)
    {
        WidgetDraw(WIDGET(Laby), NULL);

        ThreadTimerWait(10);

        Fade++;
    }

    while (Fade > 0)
    {
        WidgetDraw(WIDGET(Laby), NULL);

        ThreadTimerWait(10);

        Fade--;
    }

    WIDGET(Laby)->Draw = &GameDraw;

    WidgetDraw(WIDGET(Laby), NULL);
}

_PUBLIC void  NextLevel ( void )
{
    WhereX = 1;
    WhereY = 1;
    WeaponActive = 0;
    CurrentEnemyCount++;
    if (CurrentEnemyCount > MAX_ENEMIES) CurrentEnemyCount = MAX_ENEMIES;
    InitBombs();
    InitWeaponBox();
    if (Level < 4) {
        Level++;
        memcpy(MapDAT, MapDATLevels[Level - 1], sizeof(MapDAT));
    } else {
        MessageBox(&Me, "Congratulations!", "You have completed all levels!", MBB_OK);
    }

    FadeEffect();
    InitEnemies();
}

_PUBLIC void  UpdateMove ( void )
{
    int i;

    if (MapDAT[WhereX][WhereY] == 2)
    {
        MessageBox(&Me, "Winner!", "Congratulations, you have found the exit!\n\nProceed to the next level...", MBB_OK);
        NextLevel();
    }

    if (!WeaponActive && WhereX == WeaponX && WhereY == WeaponY)
    {
        WeaponActive = 1;
        MessageBox(&Me, "Weapon Activated", "You have picked up the weapon! Press Space to drop bombs.", MBB_OK);
    }

    for (i = 0; i < CurrentEnemyCount; i++)
    {
        if (Enemies[i].active && WhereX == Enemies[i].x && WhereY == Enemies[i].y)
        {
            MessageBox(&Me, "Game Over", "You were caught by an enemy!", MBB_OK);
            Level--; // Decrease level to repeat the current level
            CurrentEnemyCount--; // Decrease enemy count
            if (CurrentEnemyCount < 2) CurrentEnemyCount = 2;
            memcpy(MapDAT, MapDATLevels[Level], sizeof(MapDAT));
            InitEnemies();
            InitWeaponBox();
            InitBombs();
            FadeEffect();
            return;
        }
    }

    CheckBombs();
    WidgetDraw(WIDGET(Laby), NULL);
}

l_bool GameEH(PWidget o, PEvent Event)
{
    if (Event->Type == EV_KEYBOARD)
    {
        if (SCANCODE(Event, KEY_ENTER) && !StartGame)
        {
            StartGame = true;
            Level = 1;
            memcpy(MapDAT, MapDATLevels[0], sizeof(MapDAT));
            InitEnemies();
            InitWeaponBox();
            InitBombs();
            FadeEffect();
            return true;
        }

        if (StartGame)
        {
            if (SCANCODE(Event, KEY_RIGHT))
            {
                if (MapDAT[WhereX + 1][WhereY] != 0)
                {
                    WhereX++;
                    MoveEnemies();
                    UpdateMove();
                }

                return true;
            }

            if (SCANCODE(Event, KEY_LEFT))
            {  // arrow LEFT was pressed
                if (MapDAT[WhereX - 1][WhereY] != 0)
                {
                    WhereX--;
                    MoveEnemies();
                    UpdateMove();
                }

                return true;
            }

            if (SCANCODE(Event, KEY_DOWN))
            {  // arrow DOWN was pressed
                if (MapDAT[WhereX][WhereY + 1] != 0)
                {
                    WhereY++;
                    MoveEnemies();
                    UpdateMove();
                }

                return true;
            }

            if (SCANCODE(Event, KEY_UP))
            {  //arrow UP was pressed
                if (MapDAT[WhereX][WhereY - 1] != 0)
                {
                    WhereY--;
                    MoveEnemies();
                    UpdateMove();
                }

                return true;
            }

            if (SCANCODE(Event, KEY_SPACE) && WeaponActive)
            {
                DropBomb();
                UpdateMove();
                return true;
            }
        }
    }

    return false;
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
    if ( Event->Type == EV_MESSAGE )
    {
        switch ( Event->Message )
        {
            case WM_CLOSE:
            {
                WidgetDispose(WIDGET(o));
                CloseApp(&Me);
                return true;
            }
            break;

            case WM_ABOUT:
            {
                MessageBox(&Me, "About Laby", "Laby 1.0\n\nAuthor: Lukas Lipka\n\nCopyright (c) 2003-2004 Lukas Lipka. All rights reserved.", MBB_OK);
                return true;
            }
            break;
        }
    }

    return false;
}

l_int Main ( int argc, l_text *argv )
{
    PWindow w   = 0;
    TRect r;

    RectAssign(&r,0, 0, 250, 200);
    w = CreateWindow(&Me, r, "Laby: The Dawn of Adventure", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
    InsertWidget(WIDGET(DeskTop), WIDGET(w));
    WidgetPreCenter(WIDGET(w));

    RectAssign(&r, 0, 0, 250, 200);
    Laby = CreateCanvas(&Me, r);
    WIDGET(Laby)->Draw = &GameDraw;
    WIDGET(Laby)->EventHandler = &GameEH;
    WIDGET(Laby)->Flags |= WFFocusable;
    WIDGET(Laby)->Flags |= WFForceBuffer;
    InsertWidget(WIDGET(w), WIDGET(Laby));

    WidgetDrawAll(WIDGET(w));

    WidgetSetFocus(WIDGET(Laby));

    WIDGET(w)->AppEvHdl = &AppEventHandler;

    // Show the start menu
    WIDGET(Laby)->Draw = &FadeEffectDraw;
    WidgetDraw(WIDGET(Laby), NULL);

    return true;
}

void Close (void)
{

}
