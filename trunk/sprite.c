#include "sprite.h"


char *read_xpm(char *map[], int *wd, int *ht)
{
  __attribute__((unused)) static char read_xpm_jcard;

  int width, height, colors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol;

  // read width, height, colors
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > HRES || height > VRES || colors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  for (i=0; i<256; i++)
    sym[i] = 0;

  // read symbols <-> colors 
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 256) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }
  
  // allocate pixmap memory
  pix = pixtmp = malloc(width*height);

  // parse each pixmap symbol line
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 256);  // find color of each symbol
      if (tmp == NULL) {
	printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
	return NULL;
      }
      *pixtmp++ = tmp - sym; // pointer arithmetic! back to books :-)
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }

  return pix;
}


Sprite * createSprite(int speed_x, int speed_y, int pos_x, int pos_y, char *pic[], char *base) {
	int width, height;
	Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));
	
	char *sprite_char = read_xpm(pic, &width, &height);

	
	sprite->width = width;
	sprite->height = height;
	sprite->map = sprite_char;
	sprite->x = pos_x;
	sprite->y = pos_y;
	sprite->xspeed = speed_x;
	sprite->yspeed = speed_y;
	sprite->times = 0;
	sprite->dir = 0;
	sprite->visible = true;
	
	return sprite;
}


//DESENHA A SPRITE
void drawSprite(Sprite *sprite, char *base) {
	int i;
	int j;
	int z = 0;
	
	for(i = 0; i < sprite->height; i++) {
		for(j = 0; j < sprite->width; j++) {
			if(sprite->map[z] != BLACK) {
				set_pixel(j+sprite->x,i+sprite->y, sprite->map[z],base);
			}
			z++;
		}
	}
}

//APAGA SPRITE DO ECRÃ
void destroySprite(Sprite *sprite, char *base) {
	int i;
	int j;
	for(i = 0; i < sprite->height; i++) {
		for(j = 0; j < sprite->width; j++) {
			if(get_pixel(j+sprite->x,i+sprite->y,base) != BLACK)
				set_pixel(j+sprite->x,i+sprite->y, BLACK,base);
		}
	}
}

//DESTROI SPRITE
void deleteSprite(Sprite *sprite)
{
	free(sprite->map);
	free(sprite);
}


