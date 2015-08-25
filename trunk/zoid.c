#include "list.h"
#include "zoid.h"

extern char *zoid_char1 , *zoid_char2;

int animate_zoid(Sprite *zoid, char *base) {

	int check;
	if (!zoid->visible) return -1;
	
	destroySprite(zoid, base);
	check = check_zoid_colision(zoid, zoid->x , zoid->y + zoid->yspeed, base);
	
	if ( check != 0)
		return check;
	
	if (zoid->times == 0) {
		if (zoid->map == zoid_char1) zoid->map = zoid_char2;
		else zoid->map = zoid_char1;
		zoid->times = MOVE_TIMES;
	}
	zoid->times--;
		
	zoid->y += zoid->yspeed;
	
	drawSprite(zoid, base);
	
	return check;
}


/**
 *	1 -> Passou da linha final
 *	2 -> Colidiu com espermicida
 *
 *
 */
int check_zoid_colision(Sprite *zoid, int x, int y, char* base) {

	if (y > VRES - 90) return 1;
	
	int limitx = x + zoid->width;
	int limity = y + zoid->height;

	for(; x < limitx; x++) {
		for( y /= 2; y < limity; y++)
			if (get_pixel(x, y, base) == SPMCD_COLOR) {
				//score += 500;
				return 2;	
			}
	}
	
	return 0;
}


