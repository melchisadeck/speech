#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "utterance.h"
#include "ticktock.h"

int main(argc, argv)
int argc;
char **argv;
{
  int i, u;
  u_data utt_data[NUM_UTTERANCE];
  struct timeval sub_time;
  FILE *fp = fopen("speech.d", "r");

  int u_lo = -1;
  int u_hi = -1;

  /* modify defaults according to command line switches */
  while ((i = getopt(argc, argv, "u:n:")) != -1)
    switch(i) {
    case 'u':
      u_lo = atoi(optarg);
      break;
    case 'n':
      u_hi = atoi(optarg);
      break;
    default:
      break;
    }

  if (u_lo == -1 && u_hi == -1) {
    u_lo = 0;
    u_hi = NUM_UTTERANCE;
  } else if (u_lo == -1 && u_hi != -1) {
    u_lo = 0;
    u_hi = u_hi;
  } else if (u != -1 && u_hi == -1) {
    u_lo = u_lo;
    u_hi = u_lo + 1;
  } else if (u != -1 && u_hi != -1) {
    u_lo = u_lo;
    u_hi = u_lo + u_hi;
  }

  /* scan speech.d to init utt_data[] and report time taken */
  tick(&sub_time);
  printf("init_utterance...");
  fflush(stdout);

  init_utterance(fp, utt_data);

  printf("done in %.2fs\n", tock(sub_time));
  fflush(stdout);

  /* dump utterance(s) to stdout */
  for (u = u_lo; u < u_hi; u++) {
    utt_data[u].data = read_utterance(fp, utt_data, u);
    print_utterance(utt_data, u);
    free(utt_data[u].data);
  }

  fclose(fp);

  return 0;
}
