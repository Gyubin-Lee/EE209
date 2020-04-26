#ifndef _STR_H_
#define _STR_H_

size_t StrGetLength(const char *pcSrc);
char *StrCopy(char *dest, const char *src);
int StrCompare(const char *s1, const char *s2);
char *StrSearch(const char *haystack, const char *needle);
char *StrConcat(char *dest, const char *src);

#endif
