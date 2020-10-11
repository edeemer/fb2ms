#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "util.h"

void
addel(Elements *e, const char *s)
{
	if(e->n == 0) {
		if(e->el == 0) {
			e->el = emalloc(sizeof(char *));
		}
	}
	else
		e->el = erealloc(e->el, (e->n + 1) * sizeof(char *));
	e->n++;
	e->el[e->n - 1] = emalloc(strlen(s) + 1);
	strcpy(e->el[e->n - 1], s);
}

void
rmel(Elements *e)
{
	if(e->n == 0)
		return;
	free(e->el[e->n - 1]);
	e->n--;
	if(e->n == 0) {
		free(e->el);
		e->el = 0;
		return;
	}
	e->el = erealloc(e->el, e->n * sizeof(char*));
}

char *
curel(Elements *e)
{
	return e->el[e->n - 1];	
}

int
isin(Elements *e, char *s)
{
	int i;
	
	for(i = 0; i < e->n; i++) {
		if(strcmp(e->el[i], s) == 0)
			return 1;
	}
	return 0;
}
