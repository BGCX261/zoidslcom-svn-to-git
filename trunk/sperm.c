#include "sperm.h"
#include "list.h"

extern list_t *spermicides;

void animate_spermicides(char *base) {

	if (spermicides->first == NULL) return;

	lst_iitem_t* curSp;
	
	int n = 0;
	for(curSp = spermicides->first; curSp != NULL; curSp = curSp->next)
		n++;
	
	for(curSp = spermicides->first; curSp != NULL; curSp = curSp->next){
		destroySprite(curSp->spr, base);
		
		if (!check_spermicide_colision(curSp->spr,curSp->spr->y - curSp->spr->yspeed)) {
			curSp->spr->y -= curSp->spr->yspeed;
			drawSprite(curSp->spr, base);
		}
		else {
			deleteSprite(curSp->spr);
			curSp = lst_remove(spermicides, curSp->spr);
			if (curSp == NULL) break;
		}
	}
	
}

int check_spermicide_colision(Sprite *spermicide, int next_y) {

	if (next_y < - (spermicide->height) - spermicide_speed) {
		//spermicide->visible = false;
		return 1;
	}
	return 0;

}

