#include "config.h"
#include <allegro5/allegro.h>

typedef struct sprites
{
    ALLEGRO_BITMAP *sheet;

    ALLEGRO_BITMAP *ship;
    ALLEGRO_BITMAP *life;

    ALLEGRO_BITMAP *weak_enemy[2];
    ALLEGRO_BITMAP *medium_enemy[2];
    ALLEGRO_BITMAP *strong_enemy[2];
    ALLEGRO_BITMAP *mother_ship;

    ALLEGRO_BITMAP *weak_enemy_shot[3];
    ALLEGRO_BITMAP *medium_enemy_shot[2];
    ALLEGRO_BITMAP *strong_enemy_shot[4];

    ALLEGRO_BITMAP *obstacle[5];

    ALLEGRO_BITMAP *enemy_explosion[2];
    ALLEGRO_BITMAP *shot_explosion[2];
    ALLEGRO_BITMAP *ship_explosion[2];

    ALLEGRO_BITMAP *powerup[2];
} sprites_t;


ALLEGRO_BITMAP *sprite_get(ALLEGRO_BITMAP *sprites, int x, int y, int w, int h);

void sprites_init(sprites_t *sprites);

void sprites_deinit(sprites_t *sprites);