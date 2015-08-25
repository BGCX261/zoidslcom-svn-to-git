#include "ovule.h"

void move_ovule(char side, Sprite *ovule, char *base) {
	destroySprite(ovule, base);
	
	switch(side) {
		case 'l':
			if(ovule->x - ovule_speed >= 0 && ovule->x - ovule_speed <= main_frame_w - ovule->width)
				ovule->x-= ovule_speed;
			else ovule->x = 2;
			break;
		case 'r':
			if(ovule->x + ovule_speed >= 0 && ovule->x + ovule_speed <= main_frame_w - ovule->width)
				ovule->x+= ovule_speed;
			else ovule->x = main_frame_w - ovule->width;
			break;
	}

	drawSprite(ovule, base);
}

void move_ovule_mouse(int x_pos, Sprite *ovule, char *base) {
	destroySprite(ovule, base);
	
	
					if(x_pos >= 2 && x_pos <= main_frame_w - ovule->width)
					ovule->x = x_pos;
				else if(x_pos < 2)
					ovule->x = 2;
				else ovule->x = main_frame_w - ovule->width;
/*	switch(side) {
		case 'l':
			if(ovule->x - ovule_speed >= 0 && ovule->x - ovule_speed <= main_frame_w - ovule->width)
				ovule->x-= ovule_speed;
			else ovule->x = 2;
			break;
		case 'r':
			if(ovule->x + ovule_speed >= 0 && ovule->x + ovule_speed <= main_frame_w - ovule->width)
				ovule->x+= ovule_speed;
			else ovule->x = main_frame_w - ovule->width;
			break;
	}
*/
	drawSprite(ovule, base);
}

