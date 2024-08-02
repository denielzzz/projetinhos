#include "shot.h"

void ship_shot_init(shot_t *shot)
{
    for(int i = 0; i < SHOT_N; i++)
    {
        shot[i].dy = -SHIP_SHOT_SPEED;
        shot[i].alive = 0;
        shot[i].frame = 0;
        shot[i].dead_frame = 0;
        shot[i].damage = 1;
    }
}

void enemy_shot_init(shot_t *shot)
{
    for(int i = 0; i < SHOT_N; i++)
    {
        shot[i].alive = 0;
        shot[i].frame = 0;
        shot[i].dead_frame = 0;
    }
}

int ship_shot_fire(shot_t *shot, ship_t *ship)
{
    if(!ship->lives)
        return 0;

    if(!shot->alive)
    {
        shot->x = ship->x;
        shot->y = ship->y;
        shot->alive = 1;
        shot->frame = 0;
        shot->dead_frame = 0;
        shot->frames = SHIP_SHOT;
        ship->shot_cooldown = 10;
    }
    else
        return 0;
    return 1;
}

int enemy_shot_fire(shot_t *shot, enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS], int x, int y, int dificulty)
{
    // retorna 0 se o inimigo for do tipo fraco ou médio e tiver um inimigo a sua frente
    if(enemy[x][y].type != STRONG_ENEMY)
    {
        for(int i = x+1; i < ENEMY_LINES; i++)
            if(enemy[i][y].alive)    
                return 0;
    }

    for(int i = 0; i < SHOT_N; i++)
    {
        // verifica se o tiro está disponivel
        if(!shot[i].alive)
        {
            shot[i].x = enemy[x][y].x;
            shot[i].y = enemy[x][y].y;
            shot[i].alive = 1;
            shot[i].frame = 0;
            shot[i].dead_frame = 0;
            // aumenta a velocidade do tiro de acordo com a dificuldade
            shot[i].dy = ENEMY_SHOT_SPEED + dificulty/4;
            if(enemy[x][y].type == WEAK_ENEMY)
            {
                shot[i].frames = WEAK_SHOT;
                shot[i].damage = 1;
            }
            else if(enemy[x][y].type == MEDIUM_ENEMY)
            {
                shot[i].frames = MEDIUM_SHOT;
                shot[i].damage = 2;
            }
            else if(enemy[x][y].type == STRONG_ENEMY)
            {
                shot[i].frames = STRONG_SHOT;
                shot[i].damage = 2;
            }
            // retorna 0 se já existir um tiro na mesma coluna
            if(i > 0 && ((shot[i].x - ENEMY_W*2 >= shot[i-1].x && shot[i].x + ENEMY_W*2 <= shot[i-1].x) 
            || (shot[i].x - ENEMY_W*2 <= shot[i-1].x && shot[i].x + ENEMY_W*2 >= shot[i-1].x)))
            {
                shot[i].alive = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

void shot_update(shot_t *shot)
{
    for(int i = 0; i < SHOT_N; i++)
    {
        if(shot[i].alive)
        {
            shot[i].y += shot[i].dy;
            // faz a animação do tiro conforme o tipo
            if(shot[i].frame < (int)shot[i].frames*5-1)
                shot[i].frame++;
            else
                shot[i].frame = 0;

            if(shot[i].y <= -5 || shot[i].y >= BUFFER_H + 5)
            {
                shot[i].alive = 0;
            }
        }
    }
}

// retorna 1 se houve colisão, 0 caso contrário
int collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    // verifica os casos em que não há colisão
    if(ax1 > bx2 || ax2 < bx1 || ay1 > by2 || ay2 < by1) 
        return 0;
    return 1;
}

void collide_update(shot_t *shot, enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS], enemy_t *mothership, ship_t *ship, obstacle_t *obstacle)
{
    for(int i = 0; i < SHOT_N; i++)
    {
        if(shot[i].alive)
        {
            // verifica se o tiro da nave colidiu com algum inimigo
            if(shot[i].frames == SHIP_SHOT)
            {
                for(int j = 0; j < ENEMY_LINES; j++)
                {
                    for(int k = 0; k < ENEMY_COLUNS; k++)
                    {
                        if(enemy[j][k].alive)
                        {
                            if(collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, enemy[j][k].x - ENEMY_W/2, enemy[j][k].y - ENEMY_H/2, enemy[j][k].x + ENEMY_W/2, enemy[j][k].y + ENEMY_H/2))
                            {
                                enemy[j][k].alive = 0;
                                shot[i].alive = 0;
                                ship->score += enemy[j][k].score;
                            }
                        }
                    }
                }
                // verifica se o tiro da nave colidiu com a nave mãe
                if(collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, mothership->x - ENEMY_W/2, mothership->y - ENEMY_H/2, mothership->x + ENEMY_W/2, mothership->y + 3))
                {
                    mothership->alive = 0;
                    shot[i].alive = 0;
                    ship->score += mothership->score;
                }
            }
            // verifica se o tiro de algum inimigo colidiu com a nave
            else if(shot[i].frames == MEDIUM_SHOT || shot[i].frames == WEAK_SHOT || shot[i].frames == STRONG_SHOT)
            {
                if(!ship->invincible_timer)
                {
                    if(collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, ship->x - SHIP_W/2, ship->y - SHIP_H/4, ship->x + SHIP_W/2, ship->y + SHIP_H/2))
                    {
                        if(ship->lives > 0)
                        {
                            ship->lives--;
                            ship->respawn_timer = 120;
                            ship->invincible_timer = 120;
                            shot[i].alive = 0;
                        }
                    }
                }
                else if(collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, ship->x - SHIP_W/2 - 2, ship->y - SHIP_H, ship->x + SHIP_W/2 + 2, ship->y + SHIP_H))
                    shot[i].alive = 0;

            }
            // verifica se algum tiro colidiu com algum obstáculo
            for(int j = 0; j < OBSTACLE_N; j++)
            {
                if(obstacle[j].life > 0)
                {
                    if(collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, obstacle[j].x - OBSTACLE_W/2, obstacle[j].y - OBSTACLE_H/2, obstacle[j].x - OBSTACLE_W/6, obstacle[j].y + OBSTACLE_H/3 + 3)
                    || collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, obstacle[j].x - OBSTACLE_W/6, obstacle[j].y - OBSTACLE_H/2, obstacle[j].x + OBSTACLE_W/6, obstacle[j].y - OBSTACLE_H/6 + 3)
                    || collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, obstacle[j].x + OBSTACLE_W/6, obstacle[j].y - OBSTACLE_H/2, obstacle[j].x + OBSTACLE_W/2, obstacle[j].y + OBSTACLE_H/3 + 3))                    
                    {
                        obstacle[j].life -= shot[i].damage;
                        shot[i].alive = 0;
                    }
                }
            }
        }
    }
}

// verifica se dois tiros colidiram
void shots_collide(shot_t *shot, shot_t *shot2)
{
    for(int i = 0; i < SHOT_N; i++)
    {
        if(shot[i].alive)
        {
            for(int j = 0; j < SHOT_N; j++)
            {
                if(shot2[j].alive)
                {
                    if(collide(shot[i].x - SHOT_W/2, shot[i].y - SHOT_H/2, shot[i].x + SHOT_W/2, shot[i].y + SHOT_H/2, shot2[j].x - SHOT_W/2, shot2[j].y - SHOT_H/3, shot2[j].x + SHOT_W/2, shot2[j].y + SHOT_H/2))
                    {
                        shot[i].alive = 0;
                        shot2[j].alive = 0;
                    }
                }
            }
        }
    }
}

// verifica se a nave colidiu com algum powerup
void powerup_collide(powerup_t *powerup, ship_t *ship)
{
    if(collide(powerup->x - SHIP_W/2, powerup->y - SHIP_H/2, powerup->x + SHIP_W/2, powerup->y + 3, ship->x - SHIP_W/2, ship->y - SHIP_H/2, ship->x + SHIP_W/2, ship->y + SHIP_H/2))
    {
        powerup->alive = 0;
        if(powerup->type == DOUBLE_SHOT)
            ship->double_shot_timer = 600;
        else if(powerup->type == INVINCIBLE)
            ship->invincible_timer = 300;
    }
}