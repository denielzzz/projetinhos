#include "map.h"
#include <stdlib.h>

void stars_init(star_t* stars)
{
    for(int i = 0; i < STARS_N; i++)
    {
        stars[i].y = rand() % BUFFER_H;
        // inicializa a velocidade de cada estrela com um valor aleatório
        stars[i].speed = (float)rand() / (float)RAND_MAX * (0.9);
    }
}

void stars_update(star_t* stars)
{
    for(int i = 0; i < STARS_N; i++)
    {
        stars[i].y += stars[i].speed;
        // se a estrela passar do limite inferior da tela, ela volta para o topo
        if(stars[i].y >= BUFFER_H)
        {
            stars[i].y = 0;
            stars[i].speed = (float)rand() / (float)RAND_MAX * (0.9);
        }
    }
}   