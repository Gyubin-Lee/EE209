#include <stdio.h>
#include <assert.h>

size_t StrGetLength(const char *pcSrc){
  int i = 0;
  const char *temp;
  temp = pcSrc;
  
  assert(pcSrc != NULL);

  while(*temp != '\0'){
    i++;
    temp++;
  }

  return (size_t)i;
}

char *StrCopy(char *dest, const char *src){
  char* dest_temp;
  const char* src_temp;
  dest_temp = dest;
  src_temp = src;

  assert(dest != NULL);
  assert(src != NULL);

  while(*src_temp != '\0'){
    *dest_temp = *src_temp;
    dest_temp++;
    src_temp++;
  }
  *dest_temp = '\0';

  return dest;
}

int StrCompare(const char *s1, const char *s2){
  const char* s1_temp;
  const char* s2_temp;
  s1_temp = s1;
  s2_temp = s2;

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
  const char* h_temp;
  const char* n_temp;
  h_temp = haystack;
  n_temp = needle;

  while(*h_temp != '\0'){
    if(*h_temp == *n_temp){
      const char* h_compare;
      const char* n_compare;
      h_compare = h_temp+1;
      n_compare = n_temp+1;

      while(*h_compare == *n_compare && *n_compare != '\0'){
	h_compare++;
	n_compare++;
      }

      if(*n_compare == '\0') return h_temp;
    }
    h_temp++;
  }
}

char *StrConcat(char *dest, const char *src){
  const char *src_temp;
  char *dest_temp;
  src_temp = src;
  dest_temp = dest;

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
  
int main(){
  char p[] = "What the hell";
  unsigned long len = StrGetLength(p);
  printf("%ld\n", len);

  char a[] = "I am a good boy";
  char b[200];

  char *res = StrCopy(b, a);

  printf("%s\n", res);

  char *c = "kkkk";
  char *d = "kkkkk";

  printf("%d\n", StrCompare(c, d));

  char *e = "abcdefgabcde";
  char *f = "f";

  printf("%s\n", StrSearch(e, f));

  char g[200] = "abcdefg";
  char *h = "hijklmn";

  printf("%s\n", StrConcat(g, h));
  
  return 0;
}
