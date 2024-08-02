#include "config.h"
#define STARS_N ((BUFFER_W / 2) - 1)

typedef struct star
{
    float y;
    float speed;
} star_t;

void stars_init(star_t* stars);

void stars_update(star_t* stars);