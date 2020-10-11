#include <ctype.h>
#include <expat.h>
#include <stdio.h>
#include <string.h>
#include "element.h"

extern Elements els;

struct Author {
	char *first;
	char *middle;
	char *last;
	char *nick;
};
typedef struct Author Author;

struct Authorlist {
	Author *a;
	struct Authorlist *next;
};
typedef struct Authorlist Authorlist;

struct Description {
	char *title;
	Authorlist *authors;
	char *annotation;
} descr;

void
printauthor(Author *a)
{
	int first = 1;

	if(a->first) {
		printf("%s", a->first);
		first = 0;
	}
	if(a->middle) {
		if(!first)
			putchar(' ');
		printf("%s", a->middle);
	}
	if(a->last) {
		if(!first)
			putchar(' ');
		printf("%s", a->last);
	}
	if(a->nick) {
		if(!first)
			putchar(' ');
		printf("%s", a->nick);
	}
	return;
}

void
freeauthors(Authorlist *list)
{
	Authorlist *prev;

	while(1) {
		if(list->a) {
			if(list->a->first)
				free(list->a->first);
			if(list->a->middle)
				free(list->a->middle);
			if(list->a->last)
				free(list->a->last);
			if(list->a->nick)
				free(list->a->nick);
			free(list->a);
		}
		if(list->next) {
			prev = list;
			list = list->next;
			free(prev);
		}
		else {
			free(list);
			break;
		}
	}	
}

/* TODO: works only with ASCII spaces, either preprocess data or include
 * libutf in this program
 */

int
strspace(char *s)
{
	int i;
	int n;
	
	n = strlen(s);
	for(i = 0; i < n; i++) {
		if(!isspace(s[i]))
			return 0;
	}
	return 1;
}

char *
trimleft(char *s)
{
	while(1) {
		if(!isspace(*s))
			return s;
		s++;
	}
}

void
descr_st()
{
	descr.title = descr.annotation = 0;

	descr.authors = malloc(sizeof(Authorlist));
	memset(descr.authors, 0, sizeof(Authorlist));
	return;
}

void
descr_end()
{
	Authorlist *al;

	printf(".TL\n%s\n", descr.title);
	al = descr.authors;
	if(al->a)
		puts(".AU");
	while(1) {
		if(al->a) {
			printauthor(al->a);
			putchar('\n');
		}
		if(al->next)
			al = al->next;
		else
			break;	
	}
	free(descr.title);
	freeauthors(descr.authors);;
	return;
}

void
author_st()
{
	Authorlist *al;

	if(isin(&els, "document-info"))
		return;
	for(al = descr.authors; al->next; al = al->next)
		printf("author skipped\n");
	if(al->a == 0) {
		al->a= malloc(sizeof(Author));
		memset(al->a, 0, sizeof(Author));
	}
	else {
		al->next = malloc(sizeof(Authorlist));
		memset(al->next, 0, sizeof(Authorlist));
		al->next->a = malloc(sizeof(Author));
		memset(al->next->a, 0, sizeof(Author));
	}
	return;
}

void
booktitle_dat(char *data)
{
	descr.title = malloc(strlen(data) + 1);
	strcpy(descr.title, data);
	return;
}

void
firstname_dat(char *data)
{
	Authorlist *al;

	if(isin(&els, "document-info"))
		return;
	for(al = descr.authors; al->next; al = al->next);
	al->a->first = malloc(strlen(data) + 1);
	strcpy(al->a->first, data);
	return;
}

void
midname_dat(char *data)
{
	Authorlist *al;

	if(isin(&els, "document-info"))
		return;
	for(al = descr.authors; al->next; al = al->next)
		printf("!author skipped\n");
	al->a->middle = malloc(strlen(data) + 1);
	strcpy(al->a->middle, data);
	return;
}

void
lastname_dat(char *data)
{
	Authorlist *al;

	if(isin(&els, "document-info"))
		return;
	for(al = descr.authors; al->next; al = al->next);
	al->a->last = malloc(strlen(data) + 1);
	strcpy(al->a->last, data);
	return;
}

void
nickname_dat(char *data)
{
	Authorlist *al;

	if(isin(&els, "document-info"))
		return;
	for(al = descr.authors; al->next; al = al->next);
	al->a->nick = malloc(strlen(data) + 1);
	strcpy(al->a->nick, data);
	return;
}

void
title_st()
{
	if(isin(&els, "description"))
		return;	
	printf(".SH\n"); 
}

void
title_end()
{
	return;
}

void
title_dat(char *data)
{
	if(isin(&els, "description") || strspace(data))
		return;
	printf("%s\n", trimleft(data));	
}

void
p_st()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	if(isin(&els, "cite"))
		printf(".QP\n");	/* is QP an extension of ms? */
	else
		printf(".PP\n");
}

void
p_end()
{
	if(isin(&els, "description"))
		return;
	printf("\n");
}

void
p_dat(char *data)
{
	if(isin(&els, "description") || strspace(data))
		return;
	printf("%s", data);
}

void
emp_st()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	if(isin(&els, "strong"))
		printf("\\f(BI");
	else
		printf("\\fI");
}

void
emp_end()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	if(isin(&els, "strong"))
		printf("\\fB");
	else
		printf("\\fR");
}

void
emp_dat(char *data)
{
	if(isin(&els, "description") || strspace(data))
		return;
	printf("%s", trimleft(data));
}

void
strong_st()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	if(isin(&els, "emphasis"))
		printf("\\f(BI");
	else
		printf("\\fB");
}

void
strong_end()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	if(isin(&els, "emphasis"))
		printf("\\fI");
	else
		printf("\\fR");
}

void
strong_dat(char *data)
{
	if(isin(&els, "description") || strspace(data))
		return;
	printf("%s", trimleft(data));
}

void
emptyline_end()
{
	if(isin(&els, "description"))
		return;
	puts("\n");
}

void
a_dat(char *data)
{
	if(isin(&els, "description") || strspace(data))
		return;
	printf("%s", data);
}

void
sup_st()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	printf("\\s-2\\v'-0.4m'");
}

void
sup_end()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	printf("\\v'0.4m'\\s+2");
}

void
sup_dat(char *data)
{
	if(isin(&els, "description") || strspace(data))
		return;
	printf("%s", trimleft(data));
}

void
sub_st()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	printf("\\s-2\\v'0.2m'");
}

void
sub_end()
{
	if(isin(&els, "description") || isin(&els, "title"))
		return;
	printf("\\v'-0.2m'\\s+2");
}

void
epigr_st()
{
	printf(".in +10\n");
}

void
epigr_end()
{
	printf(".in -10\n");
}

void
textauthor_st()
{
	printf(".br\n.ad r\n");
}

void
textauthor_end()
{
	printf("\n.br\n.ad b\n");
}

void
textauthor_dat(char *data)
{
	printf("%s", data);
}

void
poem_st()
{
	printf(".RT\n.sp 1v\n.in +10\n");
}

void
poem_end()
{
	printf(".in -10\n");
}

void
v_end()
{
	printf("\n.br\n");
}

void
stanza_end()
{
	printf(".sp 1v\n");
}
