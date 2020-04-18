#ifndef _STR_H_
#define _STR_H_
#include <unistd.h>

size_t StrGetLength(const char *pcSrc);
char *StrCopy(char *pcDest, const char *pcSrc);
int StrCompare(const char *s1, const char *s2);
const char *StrSearch(const char *haystack, const char *needle);
char *StrConcat(char *dest, const char *src);

#endif /*_STR_H_*/
