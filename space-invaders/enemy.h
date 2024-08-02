#ifndef ENEMY_LIB
#define ENEMY_LIB

#include "config.h"

#define ENEMY_W 12
#define ENEMY_H 8

#define ENEMY_LINES 5
#define ENEMY_COLUNS 11

#define ENEMY_SPEED BUFFER_W/80

typedef enum enemy_type
{
    STRONG_ENEMY = 0,
    MEDIUM_ENEMY,
    WEAK_ENEMY,
    MOTHER_SHIP,
} enemy_type_t;

typedef struct enemy
{
    enemy_type_t type;
    int x, y, dx;
    int alive;
    int frame;
    int score;
    // usado para saber se o som de morte do inimigo já foi tocado
    int sound;
} enemy_t;

void enemies_init(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS]);

void mothership_init(enemy_t *mothership);

int enemies_update(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS]);

void mothership_update(enemy_t *mothership);

int reset_verify(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS]);

#endif