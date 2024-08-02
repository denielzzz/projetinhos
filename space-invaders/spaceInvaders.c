#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "config.h"
#include "ship.h"
#include "map.h"
#include "enemy.h"
#include "obstacle.h"
#include "shot.h"
#include "sprites.h"

void must_init(bool test, const char *description)
{
    if(test) 
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void ship_draw(ship_t *ship, sprites_t *sprites, int pause, ALLEGRO_SAMPLE *sample_explosion[2])
{
    if(ship->lives == 0 && ship->respawn_timer > 0)
    {
        // faz o som da explosão tocar a cada 3 vezes
        if(!(ship->respawn_timer%40))
            al_play_sample(sample_explosion[1], 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        // faz a animação da explosão alternar entre 2 sprites varias vezes
        if(ship->frame/5 == 2)
            ship->frame = 0;
        al_draw_bitmap(sprites->ship_explosion[ship->frame/5], ship->x - SHIP_W/2, ship->y - SHIP_H/2, 0);
        // para a animação da explosão quando o jogo pausa
        if(!pause)
            ship->frame++;
        return;
    }
    if(ship->lives == 0)
        return;

    // faz a nave piscar quando invencivel
    if(ship->invincible_timer/2%3 == 1 && ship->respawn_timer)
        return;
    
    al_draw_bitmap(sprites->ship, ship->x - SHIP_W/2, ship->y - SHIP_H/2, 0);
    // desenha um "escudo" em volta da nave quando invencivel
    if(ship->invincible_timer)
        al_draw_circle(ship->x+0.5, ship->y+2, SHIP_W/3*2, al_map_rgb(65,122,255), 1);
}

void stars_draw(star_t* stars)
{
    float color;
    float star_x = 1.5;
    for(int i = 0; i < STARS_N; i++)
    {
        color = stars[i].speed * 0.8;
        // desenha as estrelas com as cores de acordo com a velocidade delas
        al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(color,color,color));
        star_x += 2;
    }
}

void enemies_draw(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS], sprites_t *sprites, int pause, ALLEGRO_SAMPLE *sample_explosion[2])
{   
    for(int i = 0; i < ENEMY_LINES; i++)
    {
        for(int j = 0; j < ENEMY_COLUNS; j++)
        {
            if(!enemy[i][j].alive && enemy[i][j].frame > 7)
                continue;

            if(!enemy[i][j].alive)
            {
                if(!enemy[i][j].sound)
                {
                    al_play_sample(sample_explosion[0], 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    // faz com que o som da explosão não toque mais de uma vez
                    enemy[i][j].sound = 1;
                }
                // esse /4 faz cada frame da animação da explosão durar mais
                al_draw_bitmap(sprites->enemy_explosion[enemy[i][j].frame/4], enemy[i][j].x - ENEMY_W/2, enemy[i][j].y - ENEMY_H/2, 0);
                // para a animação da explosão quando o jogo pausa
                if(!pause)
                    enemy[i][j].frame++;
                continue;
            }
            // desenha os inimigos de acordo com o tipo deles
            if(enemy[i][j].type == STRONG_ENEMY)
                al_draw_scaled_bitmap(sprites->strong_enemy[enemy[i][j].frame], 0, 0, 8, 8, enemy[i][j].x - ENEMY_W*3/7, enemy[i][j].y - ENEMY_H/2, 9, ENEMY_H, 0);
            else if(enemy[i][j].type == MEDIUM_ENEMY)
                al_draw_bitmap(sprites->medium_enemy[enemy[i][j].frame], enemy[i][j].x - ENEMY_W/2, enemy[i][j].y - ENEMY_H/2, 0);
            else if(enemy[i][j].type == WEAK_ENEMY)
                al_draw_bitmap(sprites->weak_enemy[enemy[i][j].frame], enemy[i][j].x - ENEMY_W/2, enemy[i][j].y - ENEMY_H/2, 0);
        }
    }
}

void obstacle_draw(obstacle_t *obstacle, sprites_t *sprites)
{
    // desenha os obstaculos de acordo com a vida deles
    for(int i = 0; i < OBSTACLE_N; i++)
    {
        if(obstacle[i].life == 10 || obstacle[i].life == 9)
            al_draw_scaled_bitmap(sprites->obstacle[0], 0, 0, 23, 17, obstacle[i].x - OBSTACLE_W/2, obstacle[i].y - OBSTACLE_H/2, OBSTACLE_W, OBSTACLE_H, 0);
        else if(obstacle[i].life == 8 || obstacle[i].life == 7)
            al_draw_scaled_bitmap(sprites->obstacle[1], 0, 0, 23, 17, obstacle[i].x - OBSTACLE_W/2, obstacle[i].y - OBSTACLE_H/2, OBSTACLE_W, OBSTACLE_H, 0);
        else if(obstacle[i].life == 6 || obstacle[i].life == 5)
            al_draw_scaled_bitmap(sprites->obstacle[2], 0, 0, 23, 17, obstacle[i].x - OBSTACLE_W/2, obstacle[i].y - OBSTACLE_H/2, OBSTACLE_W, OBSTACLE_H, 0);
        else if(obstacle[i].life == 4 || obstacle[i].life == 3)
            al_draw_scaled_bitmap(sprites->obstacle[3], 0, 0, 23, 17, obstacle[i].x - OBSTACLE_W/2, obstacle[i].y - OBSTACLE_H/2, OBSTACLE_W, OBSTACLE_H, 0);
        else if(obstacle[i].life == 2 || obstacle[i].life == 1)
            al_draw_scaled_bitmap(sprites->obstacle[4], 0, 0, 23, 17, obstacle[i].x - OBSTACLE_W/2, obstacle[i].y - OBSTACLE_H/2, OBSTACLE_W, OBSTACLE_H, 0);

    }
}

void shot_draw(shot_t *shot, sprites_t *sprites, int pause)
{
    for(int i = 0; i < 2; i++)
    {
        if(!shot[i].alive && shot[i].dead_frame > 5)
            continue;

        // desenha a colisão dos tiros
        if(!shot[i].alive)
        {
            al_draw_bitmap(sprites->shot_explosion[shot[i].dead_frame/3], shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, 0);
            // para a animação da explosão quando o jogo pausa
            if(!pause)
                shot[i].dead_frame++;
            continue;
        }
        // desenha os tiros de acordo com o tipo deles
        if(shot[i].frames == SHIP_SHOT)
            al_draw_filled_rectangle(shot[i].x - SHOT_W/2 + 1, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, al_map_rgb(255,255,255));
        else if(shot[i].frames == MEDIUM_SHOT)
            al_draw_bitmap(sprites->medium_enemy_shot[shot[i].frame/5], shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, 0);
        else if(shot[i].frames == WEAK_SHOT)
            al_draw_bitmap(sprites->weak_enemy_shot[shot[i].frame/5], shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, 0);
        else if(shot[i].frames == STRONG_SHOT)
            al_draw_bitmap(sprites->strong_enemy_shot[shot[i].frame/5], shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, 0);
    }
}

void mothership_draw(enemy_t *mothership, sprites_t *sprites, ALLEGRO_SAMPLE *sample_explosion[2], ALLEGRO_FONT *small_font)
{
    // faz o score da mothership durar na tela após ela morrer
    if(!mothership->alive && mothership->frame > 7)
    {
        if(mothership->frame > 30)
            return;
        al_draw_textf(small_font, al_map_rgb(255,255,255), mothership->x, mothership->y - 15, ALLEGRO_ALIGN_CENTER, "+%d", mothership->score);
        mothership->frame++;
        return;
    }
    if(!mothership->alive)
    {
        if(!mothership->sound)
        {
            al_play_sample(sample_explosion[0], 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            // faz com que o som da explosão não toque mais de uma vez
            mothership->sound = 1;
        }
        // esse /4 faz a cada frame da animação da explosão durar mais
        al_draw_bitmap(sprites->enemy_explosion[mothership->frame/4], mothership->x - ENEMY_W/2, mothership->y - ENEMY_H/2, 0);
        al_draw_textf(small_font, al_map_rgb(255,255,255), mothership->x, mothership->y - 15, ALLEGRO_ALIGN_CENTER, "+%d", mothership->score);
        mothership->frame++;
        return;
    }
    al_draw_bitmap(sprites->mother_ship, mothership->x - ENEMY_W/2, mothership->y - ENEMY_H/2, 0);
}

void powerup_draw(powerup_t *powerup, sprites_t *sprites)
{
    if(!powerup->alive)
        return;
    if(powerup->type == 0)
        al_draw_bitmap(sprites->powerup[0], powerup->x - 4, powerup->y - 4, 0);
    else if(powerup->type == 1)
        al_draw_bitmap(sprites->powerup[1], powerup->x - 4, powerup->y - 4, 0);
}

void hud_draw(int score, int lives, sprites_t *sprites, ALLEGRO_FONT *small_font)
{
    al_draw_textf(small_font, al_map_rgb(255,255,255), 10, 10, ALLEGRO_ALIGN_LEFT, "SCORE: %d", score);
    al_draw_textf(small_font, al_map_rgb(255,255,255), BUFFER_W*3/5 + 25, 10, ALLEGRO_ALIGN_LEFT, "LIVES: %dx", lives);
    al_draw_bitmap(sprites->ship, BUFFER_W*5/6 + 25, 9, 0);
}

void menu_draw(ALLEGRO_FONT *medium_font, ALLEGRO_FONT *small_font, long long int cont, sprites_t *sprites)
{
    // faz o fundo ficar mais escuro
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(1,1,1,80));
    al_draw_text(medium_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/5, ALLEGRO_ALIGN_CENTER, "SPACE INVADERS");

    al_draw_bitmap(sprites->weak_enemy[0], 15 + BUFFER_W/3 - ENEMY_W/2, BUFFER_H/3 - ENEMY_H/2 + 10, 0);
    al_draw_text(small_font, al_map_rgb(255,255,255), 15 + BUFFER_W/2, BUFFER_H/3 - ENEMY_H/2 + 10, ALLEGRO_ALIGN_CENTER, "= 10 PTS");

    al_draw_bitmap(sprites->medium_enemy[0], 16 + BUFFER_W/3 - ENEMY_W/2, BUFFER_H/3 - ENEMY_H/2 + 33, 0);
    al_draw_text(small_font, al_map_rgb(255,255,255), 15 + BUFFER_W/2, BUFFER_H/3 - ENEMY_H/2 + 33, ALLEGRO_ALIGN_CENTER, "= 20 PTS");

    al_draw_bitmap(sprites->strong_enemy[1], 18 + BUFFER_W/3 - ENEMY_W/2, BUFFER_H/3 - ENEMY_H/2 + 56, 0);
    al_draw_text(small_font, al_map_rgb(255,255,255), 15 + BUFFER_W/2, BUFFER_H/3 - ENEMY_H/2 + 56, ALLEGRO_ALIGN_CENTER, "= 40 PTS");

    al_draw_bitmap(sprites->mother_ship, 14 + BUFFER_W/3 - ENEMY_W/2, BUFFER_H/3 - ENEMY_H/2 + 80, 0);
    al_draw_text(small_font, al_map_rgb(255,255,255), 15 + BUFFER_W/2, BUFFER_H/3 - ENEMY_H/2 + 80, ALLEGRO_ALIGN_CENTER, " = ??? PTS");
    // faz o texto piscar a cada 1,5 sec
    if(cont % 90 < 45)
    {
        al_draw_text(small_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H*4/5, ALLEGRO_ALIGN_CENTER, "PRESS SPACE TO START");
        al_draw_text(small_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H*4/5 + 20, ALLEGRO_ALIGN_CENTER, "PRESS ESC TO EXIT");
    }
}

void pause_draw(ALLEGRO_FONT *big_font, ALLEGRO_FONT *small_font, long long int cont)
{
    // faz o fundo ficar mais escuro
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(1,1,1,80));
    al_draw_text(big_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/5, ALLEGRO_ALIGN_CENTER, "PAUSE");
    // faz o texto piscar a cada 1,5 sec
    if(cont % 90 < 45)
        al_draw_text(small_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/2 + 20, ALLEGRO_ALIGN_CENTER, "PRESS P TO CONTINUE");
}

void draw_game_over(ship_t *ship, ALLEGRO_FONT *big_font, ALLEGRO_FONT *medium_font, ALLEGRO_FONT *small_font, unsigned long long int cont)
{
    // faz o fundo ficar mais escuro
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(1,1,1,80));
    al_draw_text(big_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/5, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    al_draw_textf(medium_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/2 + 20, ALLEGRO_ALIGN_CENTER, "SCORE: %d", ship->score);
    // faz o texto piscar a cada 1,5 sec
    if(cont % 90 < 45)
    {
        al_draw_text(small_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/2 + 60, ALLEGRO_ALIGN_CENTER, "PRESS ESC TO EXIT OR");
        al_draw_text(small_font, al_map_rgb(255,255,255), BUFFER_W/2, BUFFER_H/2 + 80, ALLEGRO_ALIGN_CENTER, "SPACE TO PLAY AGAIN");
    }
}

void reset(ship_t *ship, star_t stars[STARS_N], enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS], enemy_t *mothership, obstacle_t obstacle[OBSTACLE_N], shot_t ship_shot[2], shot_t enemy_shot[2], powerup_t *powerup, int *dificulty)
{
    ship_init(ship);
    stars_init(stars);
    enemies_init(enemy);
    mothership->alive = 0;
    obstacle_init(obstacle);
    ship_shot_init(ship_shot);
    enemy_shot_init(enemy_shot);
    powerup->alive = 0;
    *dificulty = 0;
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_BITMAP* buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "buffer");
    
    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    must_init(al_init_font_addon(), "image");
    must_init(al_init_ttf_addon(), "ttf");;

    ALLEGRO_FONT* big_font = al_load_ttf_font("./fonts/ARCADE_N.TTF", 30, 0);
    ALLEGRO_FONT* medium_font = al_load_ttf_font("./fonts/ARCADE_N.TTF", 16, 0);
    ALLEGRO_FONT* small_font = al_load_ttf_font("./fonts/ARCADE_N.TTF", 8, 0);
    must_init(big_font, "font");
    must_init(medium_font, "font");
    must_init(small_font, "font");

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    ALLEGRO_SAMPLE *sample_shot;
    ALLEGRO_SAMPLE *sample_explode[2];
    sample_shot = al_load_sample("./sounds/shot.flac");
    must_init(sample_shot, "shot sample");
    sample_explode[0] = al_load_sample("./sounds/enemyexplosion.flac");
    must_init(sample_explode[0], "explode[0] sample");
    sample_explode[1] = al_load_sample("./sounds/shipexplosion.flac");
    must_init(sample_explode[1], "explode[1] sample");

    must_init(al_init_primitives_addon(), "primitives");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ship_t ship;

    star_t stars[STARS_N];
    stars_init(stars);

    enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS];
    enemy_t mothership;

    obstacle_t obstacle[OBSTACLE_N];

    shot_t ship_shot[2];

    shot_t enemy_shot[2];

    powerup_t powerup;

    sprites_t sprites;
    sprites_init(&sprites);

    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE key;
    bool done = false;
    bool redraw = true;
    int game_over = 0;
    int dificulty = 0;
    int pause = 0;
    int pause_timer = 0;
    int menu = 1;
    unsigned long long int cont = 0;

    al_start_timer(timer);

    while(1)
    {
        // conta os frames
        cont++;
        al_wait_for_event(queue, &event);

        switch(event.type)
        {

            case ALLEGRO_EVENT_TIMER:
                al_get_keyboard_state(&key);
                // caso queira recomeçar o jogo na tela de game over
                if(al_key_down(&key, ALLEGRO_KEY_SPACE) && game_over)
                {
                    reset(&ship, stars, enemy, &mothership, obstacle, ship_shot, enemy_shot, &powerup, &dificulty);
                    game_over = 0;
                }
                // caso queira sair do jogo
                if(al_key_down(&key, ALLEGRO_KEY_ESCAPE))
                    done = true;
                // caso queira começar o jogo no menu
                if(al_key_down(&key, ALLEGRO_KEY_SPACE) && menu)
                {
                    menu = 0;
                    reset(&ship, stars, enemy, &mothership, obstacle, ship_shot, enemy_shot, &powerup, &dificulty);
                }
                // caso queira pausar o jogo
                if(al_key_down(&key, ALLEGRO_KEY_P) && !pause_timer && !game_over && !menu)
                {
                    pause = !pause;
                    pause_timer = 10;
                }
                if(menu)
                {
                    stars_update(stars);
                    redraw = true;
                    break;
                }
                if(!pause)
                {
                    if(ship.lives > 0)
                    {
                        // movimentação da nave
                        if(al_key_down(&key, ALLEGRO_KEY_LEFT))
                            ship.dx = -SHIP_SPEED;
                        else if(al_key_down(&key, ALLEGRO_KEY_RIGHT))
                            ship.dx = SHIP_SPEED;
                        else
                            ship.dx = 0;
                        // tiro da nave
                        if(al_key_down(&key, ALLEGRO_KEY_SPACE) && !ship.shot_cooldown && !ship.double_shot_timer)
                        {
                            if(ship_shot_fire(&ship_shot[0], &ship))
                                al_play_sample(sample_shot, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        // tiro duplo
                        else if(al_key_down(&key, ALLEGRO_KEY_SPACE) && !ship.shot_cooldown && ship.double_shot_timer)
                        {
                            if(ship_shot_fire(&ship_shot[0], &ship))
                                al_play_sample(sample_shot, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            else if(ship_shot_fire(&ship_shot[1], &ship))
                                al_play_sample(sample_shot, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        // faz um inimigo aleatorio atirar
                        if(cont % ((rand() % 2*FPS) + 10) == 0)
                        {
                            while(1)
                            {
                                int x = rand() % ENEMY_LINES;
                                int y = rand() % ENEMY_COLUNS;
                                // verifica se o inimigo está vivo
                                if(!enemy[x][y].alive)
                                    continue;
                                // verifica se o inimigo pode atirar
                                if(enemy_shot_fire(enemy_shot, enemy, x, y, dificulty))
                                    al_play_sample(sample_shot, 0.3, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            }
                        }
                    }
                    if(ship.lives > 0 || ship.respawn_timer)
                    {
                        // para o movimento da nave se ela estiver morta
                        if(ship.lives == 0 && ship.respawn_timer)
                            ship.dx = 0;
                        stars_update(stars);
                        ship_update(&ship);
                        // atualiza os inimigos de acordo com a dificuldade
                        if(cont % (30 - 3*dificulty) == 0)
                            // verifica se os inimigos chegaram no limite da tela
                            if(enemies_update(enemy))
                                ship.lives = 0;
                        // faz a mothership aparecer em um tempo aleatorio, em media a cada 17 segundos
                        if(rand() % 1000 == 0)
                            mothership_init(&mothership);
                        // diminui a velocidade da mothership
                        if(cont%2 || cont%3)
                            mothership_update(&mothership);
                        // faz um powerup aparecer em um tempo aleatorio, em media a cada 17 segundos
                        if(rand() % 1000 == 0)
                            powerup_init(&powerup);
                        // diminui a velocidade de queda do powerup
                        if(cont%2 || cont%3)
                            powerup_update(&powerup);
                        shot_update(ship_shot);
                        shot_update(enemy_shot);
                        collide_update(enemy_shot, enemy, &mothership, &ship, obstacle);
                        collide_update(ship_shot, enemy, &mothership, &ship, obstacle);
                        powerup_collide(&powerup, &ship);
                        // verifica se todos os inimigos morreram
                        if(reset_verify(enemy))
                        {
                            enemies_init(enemy);
                            if(dificulty < 6)
                                dificulty++;
                            if(ship.lives < 5)
                                ship.lives++;
                        }
                        shots_collide(ship_shot, enemy_shot);
                        obstacle_update(obstacle);
                    }
                    else
                        game_over = 1;

                }
                if(pause_timer > 0)
                    pause_timer--;
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(10,10,10));

            if(menu)
            {
                stars_draw(stars);
                menu_draw(medium_font, small_font, cont, &sprites);
            }
            else
            {
                stars_draw(stars);
                powerup_draw(&powerup, &sprites);
                ship_draw(&ship, &sprites, pause, sample_explode);
                enemies_draw(enemy, &sprites, pause, sample_explode);
                mothership_draw(&mothership, &sprites, sample_explode, small_font);
                obstacle_draw(obstacle, &sprites);
                shot_draw(enemy_shot, &sprites, pause);
                shot_draw(ship_shot, &sprites, pause);
                hud_draw(ship.score, ship.lives, &sprites, small_font);
                if(pause)
                    pause_draw(big_font, small_font, cont);
                if(game_over)
                    draw_game_over(&ship, big_font, medium_font, small_font, cont);
            }

            al_set_target_backbuffer(disp);
            al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
            al_flip_display();
            redraw = false;
        }
    }
    sprites_deinit(&sprites);
    al_destroy_sample(sample_shot);
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_font(big_font);
    al_destroy_font(medium_font);
    al_destroy_font(small_font);

    return 0;
}