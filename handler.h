void author_st();
void descr_st();
void p_st();
void title_st();
void emp_st();
void p_end();
void strong_st();
void sup_st();
void sub_st();
void epigr_st();
void textauthor_st();
void poem_st();
void subtitle_st();
void descr_end();
void title_end();
void emp_end();
void strong_end();
void emptyline_end();
void sup_end();
void sub_end();
void epigr_end();
void poem_end();
void textauthor_end();
void v_end();
void subtitle_end();
void stanza_end();
void booktitle_dat();
void firstname_dat();
void midname_dat();
void lastname_dat();
void nickname_dat();
void p_dat();
void title_dat();
void emp_dat();
void strong_dat();
void a_dat();
void sup_dat();
void textauthor_dat();

struct handler {
	const char *name;
	void (*fstart)(void);
	void (*fend)(void);
	void (*fdata)();
};
typedef struct handler Handler;

Handler handlers[] = {
	{"author",	author_st,	0,		0},
	{"description",	descr_st,	descr_end,	0},
	{"book-title",	0,		0,		booktitle_dat},
	{"first-name",	0,		0,		firstname_dat},
	{"middle-name",	0,		0,		midname_dat},
	{"last-name",	0,		0,		lastname_dat},
	{"nickname",	0,		0,		nickname_dat},
	{"p",		p_st,		p_end,		p_dat},
	{"subtitle",	subtitle_st,	subtitle_end,	p_dat},
	{"title",	title_st,	title_end,	title_dat},
	{"emphasis",	emp_st,		emp_end,	emp_dat},
	{"strong",	strong_st,	strong_end,	strong_dat},
	{"empty-line",	0,		emptyline_end,	0},
	{"a",		0,		0,		a_dat},
	{"sup",		sup_st,		sup_end,	sup_dat},
	{"epigraph",	epigr_st,	epigr_end,	0},
	{"text-author",	textauthor_st,	textauthor_end,	textauthor_dat},
	{"sub",		sub_st,		sub_end,	sup_dat},
	{"poem",	poem_st,	poem_end,	0},
	{"v",		0,		v_end,		p_dat},
	{"stanza",	0,		stanza_end,	0},
};
