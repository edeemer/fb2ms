#include <expat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "element.h"
#include "handler.h"

#define LEN(X) (sizeof X / sizeof X[0])

Elements els;
int repeat = 0;
char *curdata = 0;

void
givedata()
{
	int i;

	if(curdata == 0)
		return;
	for(i = 0; i < LEN(handlers); i++) {
		if(strcmp(handlers[i].name, curel(&els)) == 0) {
			if(handlers[i].fdata == 0)
				break;
			handlers[i].fdata(curdata);
			free(curdata);
			curdata = 0;
		}
	}	
}

static void XMLCALL
startel(void *ud, const XML_Char *name, const XML_Char **atts)
{
	int i;

	givedata();
	addel(&els, name);
	repeat = 0;

	for(i = 0; i < LEN(handlers); i++) {
		if(strcmp(handlers[i].name, name) == 0) {
			if(handlers[i].fstart == 0)
				break;
			handlers[i].fstart();
		}
	}
}

static void XMLCALL
endel(void *ud, const XML_Char *name)
{
	int i;

	givedata();
	for(i = 0; i < LEN(handlers); i++) {
		if(strcmp(handlers[i].name, name) == 0) {
			if(handlers[i].fend == 0)
				break;
			handlers[i].fend();
		}
	}
	rmel(&els);
	repeat = 0;
}

static void
datahandler(void *ud, const XML_Char *s, int l)
{
	int i, newlen;

	for(i = 0; i < LEN(handlers); i++) {
		if(strcmp(handlers[i].name, curel(&els)) == 0) {
			if(handlers[i].fdata == 0)
				break;
			if(repeat) {
				newlen = strlen(curdata) + l + 1;
				curdata = realloc(curdata, newlen + 1);
				strncat(curdata, s, l);
				curdata[newlen] = '\0';
			}
			else {
				curdata = malloc(l + 1);
				strncpy(curdata, s, l);
				curdata[l] = '\0';
			}
			repeat = 1;
		}
	}
}

int
main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	int depth = 0;
	int done;
	int len;
	XML_Parser parser;

	memset(&els, 0, sizeof(Elements));
	
	parser = XML_ParserCreate("UTF-8");
	XML_SetUserData(parser, &depth);
	XML_SetElementHandler(parser, startel, endel);
	XML_SetCharacterDataHandler(parser, datahandler);
	do {
		len = fread(buf, 1, sizeof(buf), stdin);
		done = len < sizeof(buf);
		if(XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR)
      			return 1;
	} while(!done);
	XML_ParserFree(parser);
	return 0;
}
