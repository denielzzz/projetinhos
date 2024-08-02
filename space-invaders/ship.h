#ifndef SHIP_LIB
#define SHIP_LIB

#include "config.h"

#define SHIP_SPEED 2
#define SHIP_Y (BUFFER_H - BUFFER_H / 12)
#define SHIP_W 19
#define SHIP_H 9

typedef struct ship
{
    int x, y, dx;
    int lives;
    int respawn_timer;
    int invincible_timer;
    int double_shot_timer;
    int shot_cooldown;
    int frame;
    int score;
} ship_t;

typedef enum powerup_type
{
    DOUBLE_SHOT = 0,
    INVINCIBLE,
} powerup_type_t;

typedef struct powerup
{
    int x, y, dy;
    int alive;
    powerup_type_t type;
} powerup_t;

void ship_init(ship_t *ship);

void powerup_init(powerup_t *powerup);

void ship_update(ship_t *ship);

void powerup_update(powerup_t *powerup);

#endif