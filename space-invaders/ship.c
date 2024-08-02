#include "ship.h"
#include <stdlib.h>

void ship_init(ship_t *ship)
{
    ship->x = BUFFER_W / 2;
    ship->y = SHIP_Y;
    ship->dx = 0;
    ship->lives = 3;
    ship->respawn_timer = 0;
    ship->invincible_timer = 60;
    ship->double_shot_timer = 0;
    ship->frame = 0;
    ship->shot_cooldown = 10;
    ship->score = 0;
}

void powerup_init(powerup_t *powerup)
{
    if(powerup->alive)
        return;
    powerup->alive = 1;
    powerup->y = -5;
    powerup->dy = 1;
    // escolhe uma coluna aleatória para o powerup aparecer
    powerup->x = rand()%(BUFFER_W - 2*SHIP_W) + SHIP_W;
    // escolhe aleatóriamente um dos tipo para o powerup
    powerup->type = rand()%2;
}

void ship_update(ship_t *ship)
{
    ship->x += ship->dx;

    if(ship->x < SHIP_W / 2)
        ship->x = SHIP_W / 2;
    if(ship->x > BUFFER_W - SHIP_W / 2)
        ship->x = BUFFER_W - SHIP_W / 2;

    if(ship->respawn_timer > 0)
        ship->respawn_timer--;
    if(ship->invincible_timer > 0)
        ship->invincible_timer--;
    if(ship->double_shot_timer > 0)
        ship->double_shot_timer--;
    if(ship->shot_cooldown > 0)
        ship->shot_cooldown--;
}

void powerup_update(powerup_t *powerup)
{
    powerup->y += powerup->dy;
    if(powerup->y > BUFFER_H + 5)
        powerup->alive = 0;
}