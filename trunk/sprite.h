#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "videog.h"
//#include "character-graf.h"
#include "utypes.h"

#define MOVE_TIMES 7

extern int vres, hres;

typedef struct {
  int x, y;            ///< current sprite position
  int width, height;   ///< sprite dimensions
  int xspeed, yspeed;  ///< sprite current speed in the x and y direction
  char *map;           ///< the sprite pixmap (use read_xpm())
  int times;
  int dir;
  Bool visible;
} Sprite;


char *read_xpm(char *map[], int *width, int *height);

Sprite * createSprite(int speed_x, int speed_y, int pos_x, int pos_y, char *pic[], char *base);

void destroySprite(Sprite *fig, char *base);

void drawSprite(Sprite *sprite, char *base);

void deleteSprite(Sprite *sprite);

#endif
