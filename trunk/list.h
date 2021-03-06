/*
 * list.h - definitions and declarations of the integer list 
 */


#include <stdlib.h>
#include <stdio.h>
#include "sprite.h"



/* lst_iitem - each element of the list points to the next element */
typedef struct lst_iitem {
   Sprite *spr;
   struct lst_iitem *next;
} lst_iitem_t;

/* list_t */
typedef struct {
   lst_iitem_t * first;
} list_t;



/* lst_new - allocates memory for list_t and initializes it */
list_t* lst_new();

/* lst_destroy - free memory of list_t and all its items */
void lst_destroy(list_t *);

/* lst_insert - insert a new item with value 'value' in list 'list' */
void lst_insert(list_t *list, Sprite *spr);

/* lst_remove - remove first item of value 'value' from list 'list' and returns a pointer to the next item*/
lst_iitem_t* lst_remove(list_t *list, Sprite *spr);

