#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *trim(char *str) {
  size_t len = 0;
  char *begp = str;
  char *endp = NULL;
  if (str == NULL) {
    return NULL;
  }
  if (str[0] == '\0') {
    return str;
  }
  len = strlen(str);
  endp = str + len;
  while (isspace((unsigned char)*begp)) {
    ++begp;
  }
  if (endp != begp) {
    while (isspace((unsigned char)*(--endp)) && endp != begp) {
    }
  }
  if (begp != str && endp == begp)
    *str = '\0';
  else if (str + len - 1 != endp)
    *(endp + 1) = '\0';
  endp = str;
  if (begp != str) {
    while (*begp) {
      *endp++ = *begp++;
    }
    *endp = '\0';
  }
  return str;
}

int main(int argc, char *argv[]) {

  FILE *bf = fopen("dm500.dat", "r");
  FILE *obf = fopen("frb.sim", "w");
  char fmt[64];
  char tmp[1024];
  fread(&tmp, sizeof(char), 64, bf);
  strcpy(fmt, trim(tmp));

  /* Initialise disposable variables. */
  int nf;
  int pos;
  float f1;
  float f2;
  float dt;
  float t1;
  float t2;
  float raj;
  float decj;
  int useang;
  long seed;
  int haslabels;
  char name[128];
  char posf[128];
  char projid[128];
  char source[128];
  char observer[128];
  char telescope[128];

  if (strcmp(fmt, "FORMAT 1") == 0) {
    fread(&name, sizeof(char), 128, bf);
    fread(&t1, sizeof(float), 1, bf);
    fread(&t2, sizeof(float), 1, bf);
    fread(&dt, sizeof(float), 1, bf);
    fread(&f1, sizeof(float), 1, bf);
    fread(&f2, sizeof(float), 1, bf);
    fread(&nf, sizeof(int), 1, bf);
    fread(&raj, sizeof(float), 1, bf);
    fread(&decj, sizeof(float), 1, bf);
    fread(&useang, sizeof(int), 1, bf);
    fread(&seed, sizeof(long), 1, bf);
  } else if (strcmp(fmt, "FORMAT 1.1") == 0) {
    fread(&name, sizeof(char), 128, bf);
    fread(&t1, sizeof(float), 1, bf);
    fread(&t2, sizeof(float), 1, bf);
    fread(&dt, sizeof(float), 1, bf);
    fread(&f1, sizeof(float), 1, bf);
    fread(&f2, sizeof(float), 1, bf);
    fread(&nf, sizeof(int), 1, bf);
    fread(&raj, sizeof(float), 1, bf);
    fread(&decj, sizeof(float), 1, bf);
    fread(&useang, sizeof(int), 1, bf);
    fread(&seed, sizeof(long), 1, bf);
    fread(&haslabels, sizeof(int), 1, bf);
    if (haslabels == 1) {
      printf("Don't support labels yet.");
      exit(1);
    }
  } else if (strcmp(fmt, "FORMAT 1.2") == 0) {
    fread(&name, sizeof(char), 128, bf);
    fread(&t1, sizeof(float), 1, bf);
    fread(&t2, sizeof(float), 1, bf);
    fread(&dt, sizeof(float), 1, bf);
    fread(&f1, sizeof(float), 1, bf);
    fread(&f2, sizeof(float), 1, bf);
    fread(&nf, sizeof(int), 1, bf);
    fread(&pos, sizeof(int), 1, bf);
    if (pos == 1) {
      fread(&raj, sizeof(float), 1, bf);
      fread(&decj, sizeof(float), 1, bf);
    } else
      fread(&posf, sizeof(char), 128, bf);
    fread(&useang, sizeof(int), 1, bf);
    fread(&seed, sizeof(long), 1, bf);
    fread(&haslabels, sizeof(int), 1, bf);
    if (haslabels == 1) {
      printf("Don't support labels yet.");
      exit(1);
    }
  } else if (strcmp(fmt, "FORMAT 2.1") == 0) {
    fread(&name, sizeof(char), 128, bf);
    fread(&t1, sizeof(float), 1, bf);
    fread(&t2, sizeof(float), 1, bf);
    fread(&dt, sizeof(float), 1, bf);
    fread(&f1, sizeof(float), 1, bf);
    fread(&f2, sizeof(float), 1, bf);
    fread(&nf, sizeof(int), 1, bf);
    fread(&pos, sizeof(int), 1, bf);
    if (pos == 1) {
      fread(&raj, sizeof(float), 1, bf);
      fread(&decj, sizeof(float), 1, bf);
    } else
      fread(&posf, sizeof(char), 128, bf);
    fread(&useang, sizeof(int), 1, bf);
    fread(&seed, sizeof(long), 1, bf);
    if (haslabels == 1) {
      printf("Don't support labels yet.");
      exit(1);
    }
  } else {
    printf("Unable to process this file format.");
    exit(1);
  }

  printf("Format:       %s\n", fmt);
  printf("Name:         %s\n", name);
  printf("t1 (sec):     %f\n", t1);
  printf("t2 (sec):     %f\n", t2);
  printf("dt (sec):  %g\n", dt);
  printf("f1 (MHz):     %f\n", f1);
  printf("f2 (MHz):     %f\n", f2);
  printf("nf:        %d\n", nf);
  printf("RAJ (rad):    %g\n", raj);
  printf("DECJ (rad):   %g\n", decj);
  printf("Use angle:    %d\n", useang);
  printf("Random seed:  %d\n", (int)seed);

  float flux = 0.0;
  while (1) {
    if (fread(&flux, sizeof(float), 1, bf) == 0) break;
    fwrite(&flux, 1, sizeof(float), obf);
  }
}
