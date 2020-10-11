#include <err.h>
#include <errno.h>
#include <stdlib.h>

void *
emalloc(size_t size)
{
	void *ret;
	ret = malloc(size);
	if(ret == 0)
		err(errno, NULL);
	return ret;
}

void *
erealloc(void *ptr, size_t size)
{
	void *ret;
	ret = realloc(ptr, size);
	if(ret == 0)
		err(errno, NULL);
	return ret;
}
