#include "sprites.h"
#include "stdio.h"
#include <allegro5/allegro_image.h>


ALLEGRO_BITMAP *sprite_get(ALLEGRO_BITMAP *sprites, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sprites, x, y, w, h);
    if(!sprite)
    {
        fprintf(stderr, "Error creating sprite\n");
        exit(1);
    }
    
    return sprite;
}

void sprites_init(sprites_t *sprites)
{
    if(!(al_init_image_addon()))
    {
        fprintf(stderr, "Error initializing image addon\n");
        exit(1);
    }
    sprites->sheet = al_load_bitmap("./sprites/spritesheet.png");
    if(!sprites->sheet)
    {
        fprintf(stderr, "Error loading spritesheet\n");
        exit(1);
    }

    sprites->ship = sprite_get(sprites->sheet, 198, 12, 19, 9);

    sprites->strong_enemy[0] = sprite_get(sprites->sheet, 128, 0, 8, 8);
    sprites->strong_enemy[1] = sprite_get(sprites->sheet, 128, 32, 8, 8);

    sprites->medium_enemy[0] = sprite_get(sprites->sheet, 459, 7, 11, 8);
    sprites->medium_enemy[1] = sprite_get(sprites->sheet, 459, 39, 11, 8);

    sprites->weak_enemy[0] = sprite_get(sprites->sheet, 32, 0, 12, 8);
    sprites->weak_enemy[1] = sprite_get(sprites->sheet, 32, 32, 12, 8);

    sprites->mother_ship = sprite_get(sprites->sheet, 416, 0, 16, 7);

    sprites->weak_enemy_shot[0] = sprite_get(sprites->sheet, 352, 0, 3, 5);
    sprites->weak_enemy_shot[1] = sprite_get(sprites->sheet, 384, 0, 3, 5);
    sprites->weak_enemy_shot[2] = sprite_get(sprites->sheet, 352, 32, 3, 5);

    sprites->medium_enemy_shot[0] = sprite_get(sprites->sheet, 0, 0, 3, 5);
    sprites->medium_enemy_shot[1] = sprite_get(sprites->sheet, 0, 32, 3, 5);

    sprites->strong_enemy_shot[0] = sprite_get(sprites->sheet, 65, 0, 3, 5);
    sprites->strong_enemy_shot[1] = sprite_get(sprites->sheet, 96, 0, 3, 5);
    sprites->strong_enemy_shot[2] = sprite_get(sprites->sheet, 65, 32, 3, 5);
    sprites->strong_enemy_shot[3] = sprite_get(sprites->sheet, 98, 32, 3, 5);

    sprites->obstacle[0] = sprite_get(sprites->sheet, 288, 0, 23, 17);
    sprites->obstacle[1] = sprite_get(sprites->sheet, 320, 0, 23, 17);
    sprites->obstacle[2] = sprite_get(sprites->sheet, 288, 32, 23, 17);
    sprites->obstacle[3] = sprite_get(sprites->sheet, 320, 32, 23, 17);
    sprites->obstacle[4] = sprite_get(sprites->sheet, 288, 64, 23, 17);

    sprites->enemy_explosion[0] = sprite_get(sprites->sheet, 480, 0, 12, 10);
    sprites->enemy_explosion[1] = sprite_get(sprites->sheet, 480, 32, 12, 10);

    sprites->shot_explosion[0] = sprite_get(sprites->sheet, 384, 34, 5, 5);
    sprites->shot_explosion[1] = sprite_get(sprites->sheet, 384, 49, 5, 5);

    sprites->ship_explosion[0] = sprite_get(sprites->sheet, 230, 12, 19, 9);
    sprites->ship_explosion[1] = sprite_get(sprites->sheet, 198, 43, 19, 9);

    sprites->powerup[0] = sprite_get(sprites->sheet, 268, 44, 8, 8);
    sprites->powerup[1] = sprite_get(sprites->sheet, 268, 12, 8, 8);

}

void sprites_deinit(sprites_t *sprites)
{
    al_destroy_bitmap(sprites->sheet);

    al_destroy_bitmap(sprites->ship);

    al_destroy_bitmap(sprites->weak_enemy[0]);
    al_destroy_bitmap(sprites->weak_enemy[1]);
    al_destroy_bitmap(sprites->medium_enemy[0]);
    al_destroy_bitmap(sprites->medium_enemy[1]);
    al_destroy_bitmap(sprites->strong_enemy[0]);
    al_destroy_bitmap(sprites->strong_enemy[1]);
    al_destroy_bitmap(sprites->mother_ship);

    al_destroy_bitmap(sprites->weak_enemy_shot[0]);
    al_destroy_bitmap(sprites->weak_enemy_shot[1]);
    al_destroy_bitmap(sprites->weak_enemy_shot[2]);
    al_destroy_bitmap(sprites->medium_enemy_shot[0]);
    al_destroy_bitmap(sprites->medium_enemy_shot[1]);
    al_destroy_bitmap(sprites->strong_enemy_shot[0]);
    al_destroy_bitmap(sprites->strong_enemy_shot[1]);
    al_destroy_bitmap(sprites->strong_enemy_shot[2]);
    al_destroy_bitmap(sprites->strong_enemy_shot[3]);

    al_destroy_bitmap(sprites->obstacle[0]);
    al_destroy_bitmap(sprites->obstacle[1]);
    al_destroy_bitmap(sprites->obstacle[2]);
    al_destroy_bitmap(sprites->obstacle[3]);
    al_destroy_bitmap(sprites->obstacle[4]);
    
    al_destroy_bitmap(sprites->enemy_explosion[0]);
    al_destroy_bitmap(sprites->enemy_explosion[1]);
    al_destroy_bitmap(sprites->shot_explosion[0]);
    al_destroy_bitmap(sprites->shot_explosion[1]);
    al_destroy_bitmap(sprites->ship_explosion[0]);
    al_destroy_bitmap(sprites->ship_explosion[1]);
    al_destroy_bitmap(sprites->powerup[0]);
    al_destroy_bitmap(sprites->powerup[1]);
}
