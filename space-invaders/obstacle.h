#ifndef OBSTACLE_LIB
#define OBSTACLE_LIB

#include "config.h"

#define OBSTACLE_W 28
#define OBSTACLE_H 20
#define OBSTACLE_Y BUFFER_H - BUFFER_H / 5

#define OBSTACLE_LIFE 10
#define OBSTACLE_N 4


typedef struct obstacle
{
    int x, y;
    int life;
} obstacle_t;

void obstacle_init(obstacle_t *obstacle);

void obstacle_update(obstacle_t *obstacle);

#endif