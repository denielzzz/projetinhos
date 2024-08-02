#include "enemy.h"
#include "stdlib.h"

void enemies_init(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS])
{
    for(int i = 0; i < ENEMY_LINES; i++)
    {
        for(int j = 0; j < ENEMY_COLUNS; j++)
        {
            enemy[i][j].type = (enemy_type_t) (i+1)/2;
            enemy[i][j].x = BUFFER_W/7 + (j+1) * BUFFER_W/ENEMY_COLUNS*2/3;
            enemy[i][j].y = BUFFER_H/7 + (i+1) * BUFFER_H/ENEMY_LINES/3;
            enemy[i][j].alive = 1;
            enemy[i][j].dx = ENEMY_SPEED;
            enemy[i][j].frame = 0;
            enemy[i][j].sound = 0;
            if(enemy[i][j].type == STRONG_ENEMY)
                enemy[i][j].score = 40;
            else if(enemy[i][j].type == MEDIUM_ENEMY)
                enemy[i][j].score = 20;
            else if(enemy[i][j].type == WEAK_ENEMY)
                enemy[i][j].score = 10;
        }
    }
}

void mothership_init(enemy_t *mothership)
{
    if(mothership->alive)
        return;
    mothership->type = MOTHER_SHIP;
    // escolhe aleatóriamente se a nave mãe vai aparecer da esquerda ou da direita
    if(rand()%2)
    {
        mothership->x = BUFFER_W + 8;
        mothership->dx = -1;
    }
    else
    {
        mothership->x = 0 - 8;
        mothership->dx = 1;
    }
    mothership->y = BUFFER_H/7;
    mothership->alive = 1;
    mothership->frame = 0;
    mothership->sound = 0;
    // escolhe aleatóriamente a pontuação da nave mãe entre 100 e 800
    mothership->score = (rand()%8 + 1)*100;
}

int enemies_update(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS])
{
    // retorna 1 se algum inimigo chegar num certo limite y   
    for(int i = 0; i < ENEMY_LINES; i++)
    {
        for(int j = 0; j < ENEMY_COLUNS; j++)
        {
            if(!enemy[i][j].alive)
                continue;
            if(enemy[i][j].y + ENEMY_H/2 >= BUFFER_H*2.9/4)
                return 1;
        }
    }

    int coli = 0;
    for(int i = 0; i < ENEMY_LINES; i++)
    {
        for(int j = 0; j < ENEMY_COLUNS; j++)
        {
            if(!enemy[i][j].alive)
                continue;
            // atualiza a posição x do inimigo
            enemy[i][j].x += enemy[i][j].dx;
            enemy[i][j].frame = !enemy[i][j].frame;
            // verifica se o inimigo colidiu com algum limite da tela
            if(enemy[i][j].x - ENEMY_W/2 <= 0 || enemy[i][j].x + ENEMY_W/2 >= BUFFER_W)
                coli = 1;
        }
    }
    // se algum inimigo colidiu com algum limite da tela, todos os inimigos voltam uma posição e descem uma linha
    if(coli)
    {
        for(int i = 0; i < ENEMY_LINES; i++)
        {
            for(int j = 0; j < ENEMY_COLUNS; j++)
            {
                enemy[i][j].dx *= -1;
                enemy[i][j].x += enemy[i][j].dx;
                enemy[i][j].y += ENEMY_H;

            }
        }
    }
    return 0;
}

void mothership_update(enemy_t *mothership)
{
    if(!mothership->alive)
        return;
    mothership->x += mothership->dx;
    // verifica se a nave mãe colidiu com algum limite da tela
    if(mothership->x < -9 || mothership->x > BUFFER_W + 8)
        mothership->alive = 0;
}

// retorna 1 se todos os inimigos estiverem mortos
int reset_verify(enemy_t enemy[ENEMY_LINES][ENEMY_COLUNS])
{
    for(int i = 0; i < ENEMY_LINES; i++)
    {
        for(int j = 0; j < ENEMY_COLUNS; j++)
        {
            if(enemy[i][j].alive)
                return 0;
        }
    }
    return 1;
}