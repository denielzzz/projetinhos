CC = gcc
LIBS = `pkg-config allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 --libs --cflags`
FLAGS = -Wall -Wextra -Werror -pedantic -std=c99
FILES = spaceInvaders.c ship.c map.c enemy.c obstacle.c shot.c sprites.c
EXEC = game

all: $(FILES)
	$(CC) $(FILES) $(FLAGS) -o $(EXEC) $(LIBS) 

clean:
	rm -f $(EXEC)
