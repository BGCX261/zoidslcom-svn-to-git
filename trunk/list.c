/*
 * list.c - implementation of the integer list functions 
 */


//#include <stdlib.h>
//#include <stdio.h>
#include "list.h"



list_t* lst_new()
{
   list_t *list;
   list = (list_t*) malloc(sizeof(list_t));
   list->first = NULL;
   return list;
}


void lst_destroy(list_t *list)
{
   free(list);
}


void lst_insert(list_t *list, Sprite *spr)
{   
   lst_iitem_t* tmp;
   tmp = (lst_iitem_t*) malloc(sizeof(lst_iitem_t));
   
   tmp->spr = spr;
   tmp->next = NULL;
   
   if( list->first == NULL )
    list->first = tmp;
   else
   {
       lst_iitem_t* curElement;
       curElement = list->first;
       
	while( curElement->next != NULL )
	  curElement = curElement->next;
	  
	curElement->next = tmp;
   }
   
   
}


lst_iitem_t* lst_remove(list_t *list, Sprite *spr)
{
	lst_iitem_t* curElement;
	lst_iitem_t* ret;
	curElement = list->first;
   
	if( list->first->spr == spr ) {
		list->first = list->first->next;
		return list->first;
	}
	curElement = list->first;
   
	while( curElement->next != NULL )
		if( curElement->next->spr == spr ) {
			curElement->next = curElement->next->next;
			ret = curElement->next;
		}
		else
			curElement = curElement->next;
	
	return ret;
}


