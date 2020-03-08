#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "utterance.h"

char *county[] = {
  "Cork", "Down", "Mayo", "Cavan", "Clare", "Derry", "Kerry", "Laois",
  "Louth", "Meath", "Sligo", "Antrim", "Armagh", "Carlow", "Dublin",
  "Galway", "Offaly", "Tyrone", "Donegal", "Kildare", "Leitrim",
  "Wexford", "Wicklow", "Kilkenny", "Limerick", "Longford", "Monaghan",
  "Fermanagh", "Roscommon", "Tipperary", "Waterford", "Westmeath",
  "No match" /* for reporting no-matches to stdout */
};

void init_utterance(fp, utt_data)
u_data *utt_data;
FILE *fp;
{
  int u, i;
  unsigned char p;

  for (u = 0; u < NUM_UTTERANCE; u++) {
    utt_data[u].length = (unsigned char) fgetc(fp);
    utt_data[u].fpos   = ftell(fp);

    for (i = 0; i < utt_data[u].length * 8; i++) {
      p = (unsigned char) fgetc(fp);
      utt_data[u].max = (utt_data[u].max > p) ? utt_data[u].max : p;
    }
  }
}

unsigned char *read_utterance(fp, utt_data, u)
int u;
u_data *utt_data;
FILE *fp;
{
  int i;
  unsigned char *utterance;

  utterance = (unsigned char *) malloc(utt_data[u].length * 8 * sizeof(unsigned char));

  fseek(fp, utt_data[u].fpos, 0);
  for (i = 0; i < utt_data[u].length * 8; i++)
    utterance[i] = (unsigned char) fgetc(fp);

  return utterance;
}

void print_utterance(utt_data, u)
int u;
u_data *utt_data;
{
  int i, j;

  printf(" %3d %3d %3d %19s\n", u, utt_data[u].length, utt_data[u].max, county[u % 32]);

  for (i = 0; i < utt_data[u].length; i++) {
    for (j = 0; j < 8; j++)
      printf(" %3d", utt_data[u].data[i * 8 + j]);

    printf("\n");
  }
}

void log_scale(utt_data, u)
int u;
u_data *utt_data;
{
  int i;
  unsigned char *p = utt_data[u].data;

  utt_data[u].max = 0;

  for (i = 0; i < utt_data[u].length * 8; i++)
    if (p[i] != 0) {
      p[i] = (int) 105.0 * log10((double) p[i]);
      utt_data[u].max = (utt_data[u].max > p[i]) ? utt_data[u].max : p[i];
    }
}

void amp_normalise(utt_data, u)
int u;
u_data *utt_data;
{
  int i;
  unsigned char *p = utt_data[u].data;

  for (i = 0; i < utt_data[u].length * 8; i++)
    p[i] = (int) (255.0 * (double) p[i] / (double) utt_data[u].max);
}
