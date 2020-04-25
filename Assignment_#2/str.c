#include <stdio.h>
#include <assert.h>
#include "str.h"

size_t StrGetLength(const char *pcSrc){
  int i = 0;
  const char *temp;
  temp = pcSrc;
  
  assert(pcSrc);

  while(*temp != '\0'){
    i++;
    temp++;
  }

  return (size_t)i;
}

char *StrCopy(char *dest, const char *src){
  char* dest_temp = dest;
  const char* src_temp = src;

  assert(dest);
  assert(src);

  while(*src_temp != '\0'){
    *dest_temp = *src_temp;
    dest_temp++;
    src_temp++;
  }
  *dest_temp = '\0';

  return dest;
}

int StrCompare(const char *s1, const char *s2){
  const char* s1_temp = s1;
  const char* s2_temp = s2;

  assert(s1);
  assert(s2);

  while(*s1_temp != '\0' && *s2_temp != '\0'){
    if (*s1_temp != *s2_temp){
      return (*s1_temp-*s2_temp);
    }
    s1_temp++;
    s2_temp++;
  }

  return (*s1_temp - *s2_temp);
}

char *StrSearch(const char *haystack, const char *needle){
  char* h_temp = (char*)haystack;
  const char* n_temp = needle;

  assert(haystack);
  assert(needle);
  
  if(*n_temp == '\0') return h_temp;

  while(*h_temp != '\0'){
    if(*h_temp == *n_temp){
      char* h_compare = h_temp+1;
      const char* n_compare = n_temp+1;

      while(*h_compare == *n_compare && *n_compare != '\0'){
	      h_compare++;
	      n_compare++;
      }

      if(*n_compare == '\0') return h_temp;
    }
    h_temp++;
  }

  return NULL;
}

char *StrConcat(char *dest, const char *src){
  const char *src_temp = src;
  char *dest_temp = dest;

  assert(dest);
  assert(src);
  
  while(*dest_temp != '\0'){
    dest_temp++;
  }

  while(*src_temp != '\0'){
    *dest_temp = *src_temp;
    dest_temp++;
    src_temp++;
  }

  *dest_temp = '\0';

  return dest;
}
