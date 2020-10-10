struct Elements {
	char **el;
	int n;
};
typedef struct Elements Elements;

void addel(Elements *e, const char *s);
void rmel(Elements *e);
char *curel(Elements *e);
int isin(Elements *e, char *s);