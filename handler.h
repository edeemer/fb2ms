void putanchor();
void a_st();
void a_end();
void a_dat();
void author_st();
void booktitle_dat();
void descr_end();
void descr_st();
void emp_dat();
void emp_end();
void emp_st();
void emptyline_end();
void epigr_end();
void epigr_st();
void firstname_dat();
void lastname_dat();
void midname_dat();
void nickname_dat();
void p_dat();
void p_end();
void poem_end();
void poem_st();
void p_st();
void stanza_end();
void strong_dat();
void strong_end();
void strong_st();
void sub_end();
void sub_st();
void subtitle_end();
void subtitle_st();
void sup_dat();
void sup_end();
void sup_st();
void textauthor_dat();
void textauthor_end();
void textauthor_st();
void title_dat();
void title_end();
void title_st();
void v_end();

struct handler {
	const char *name;
	void (*fstart)(char **);
	void (*fend)(void);
	void (*fdata)();
};
typedef struct handler Handler;

Handler handlers[] = {
	{"a",		a_st,		a_end,		a_dat},
	{"author",	author_st,	0,		0},
	{"book-title",	0,		0,		booktitle_dat},
	{"description",	descr_st,	descr_end,	0},
	{"emphasis",	emp_st,		emp_end,	emp_dat},
	{"empty-line",	0,		emptyline_end,	0},
	{"epigraph",	epigr_st,	epigr_end,	0},
	{"first-name",	0,		0,		firstname_dat},
	{"last-name",	0,		0,		lastname_dat},
	{"middle-name",	0,		0,		midname_dat},
	{"nickname",	0,		0,		nickname_dat},
	{"poem",	poem_st,	poem_end,	0},
	{"p",		p_st,		p_end,		p_dat},
	{"section",	putanchor,	0,		0},
	{"stanza",	0,		stanza_end,	0},
	{"strong",	strong_st,	strong_end,	strong_dat},
	{"sub",		sub_st,		sub_end,	sup_dat},
	{"subtitle",	subtitle_st,	subtitle_end,	p_dat},
	{"sup",		sup_st,		sup_end,	sup_dat},
	{"text-author",	textauthor_st,	textauthor_end,	textauthor_dat},
	{"title",	title_st,	title_end,	title_dat},
	{"v",		0,		v_end,		p_dat},
};
