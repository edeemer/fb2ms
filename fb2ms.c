#include <err.h>
#include <expat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "element.h"
#include "handler.h"
#include "util.h"

#define LEN(X) (sizeof X / sizeof X[0])

Elements els;
int repeat = 0;
char *curdata = 0;

static void
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
			handlers[i].fstart((char **)atts);
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
				curdata = erealloc(curdata, newlen + 1);
				strncat(curdata, s, l);
				curdata[newlen] = '\0';
			}
			else {
				curdata = emalloc(l + 1);
				strncpy(curdata, s, l);
				curdata[l] = '\0';
			}
			repeat = 1;
		}
	}
}

static int
fb2ms(FILE *f)
{
	char buf[BUFSIZ];
	int done;
	int len;
	XML_Parser parser;

	parser = XML_ParserCreate("UTF-8");
	XML_SetElementHandler(parser, startel, endel);
	XML_SetCharacterDataHandler(parser, datahandler);
	do {
		len = fread(buf, 1, sizeof(buf), f);
		done = len < sizeof(buf);
		if(XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR)
      			return 1;
	} while(!done);
	XML_ParserFree(parser);
	return 0;
}

int
main(int argc, char *argv[])
{
	FILE *f;
	int i;

	memset(&els, 0, sizeof(Elements));
	if(argc == 1) {
		if(fb2ms(stdin) != 0)
			warnx("XML parsing error");
		return 0;	
	}
	for(i = 1; i < argc; i++) {
		f = fopen(argv[i], "r");
		if(f == 0) {
			warn("%s", argv[i]);
			continue;
		}
		if(fb2ms(f) != 0)
			warnx("%s: XML parsing error", argv[i]);	
	}
	return 0;
}
