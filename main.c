#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/time.h>

#include "distance.h"
#include "utterance.h"
#include "ticktock.h"

#define TRUE  1
#define FALSE 0

int main(argc, argv)
int argc;
char **argv;
{
  int c, u, r;
  int hit, miss, no_match;
  int distance, min_unknown, min_reference, min_distance;
  char hit_miss_no_match[16];

  u_data u_tmp, utt_data[NUM_UTTERANCE];
  struct timeval run_time, sub_time;
  FILE *fp;

  /* set modifiable defaults for the speech recognition process */
  int window = 13;
  int ref_lo = 480;
  int a_flag = FALSE;
  int l_flag = FALSE;

  /* start the timers */
  tick(&run_time);
  tick(&sub_time);

  /* modify defaults according to command line switches */
  while ((c = getopt(argc, argv, "aln:r:u:v:w:")) != -1)
    switch(c) {
    case 'a':
      a_flag = TRUE;
      break;
    case 'l':
      l_flag = TRUE;
      break;
    case 'r':
      ref_lo = atoi(optarg);
      break;
    case 'w':
      window = atoi(optarg);
      break;
    default:
      break;
    }

  /* open the file and abort if it's not there */
  fp = fopen("speech.d", "r");

  if (fp == NULL) {
    fprintf(stderr, "%s can't open speech.d\n", *argv);
    exit(EXIT_FAILURE);
  }

  /* report starting parameters */
  printf("window %2d ref_lo %3d l_flag %d a_flag %d\n",
	 window, ref_lo, l_flag, a_flag);
  printf("initialising utt_data[] structure...");
  fflush(stdout);

  /* scan speech.d to set utt_data[] length, fpos and max members... */
  init_utterance(fp, utt_data);

  /* use vocab starting at ref_lo as reference vocab */
  for (u = 0; u < 32; u++) {
    u_tmp                = utt_data[u];
    utt_data[u]          = utt_data[u + ref_lo];
    utt_data[u + ref_lo] = u_tmp;
  }
  printf("done in %.2fs\n", tock(sub_time));

  /* read and norm/log scale the reference data */
  tick(&sub_time);
  printf("reading reference data...");
  fflush(stdout);

  for (u = 0; u < 32; u++) {
    utt_data[u].data = read_utterance(fp, utt_data, u);

    if (l_flag)     log_scale(utt_data, u);
    if (a_flag) amp_normalise(utt_data, u);
  }
  printf("done in %.2fs\n", tock(sub_time));

  hit = miss = no_match = 0;
  printf("initialisation took %.2fs so main loop begins...\n", tock(run_time));

  /* outer loop over all unknown utterances... */
  for (u = 32; u < NUM_UTTERANCE; u++) {
    tick(&sub_time);

    /* read and log scale/normalise the unknown utterance */
    utt_data[u].data = read_utterance(fp, utt_data, u);
    if (l_flag)        log_scale(utt_data, u);
    if (a_flag)        amp_normalise(utt_data, u);

    /* report the current unknown */
    printf("%3d %9s %2d:", u, county[u % 32], utt_data[u].length);

    /* inner loop comparing this unknown to all 32 references */
    min_distance = INT_MAX;

    for (r = 0; r < 32; r++) {
      compute_matrix     (utt_data, r, u, window);
      distance = min_path(utt_data, r, u, window);

      if (u % 32 == r)
	printf(" \033[4m%3d\033[0m", distance); /* ANSI CSI underline */
      else
	printf(" %3d", distance);               /* normal typeface    */
      fflush(stdout);

      if (distance != -1 && distance < min_distance) {
	min_unknown   = u;
	min_reference = r;
	min_distance  = distance;
      }
    } /* reference comparison inner loop complete */

    /* we're done with this unknown utterance */
    free(utt_data[u].data);

    /* no-match, hit or miss */
    if (min_distance == INT_MAX) { /* no change after 32 comparisons... */
      no_match++;
      min_distance  = -1;          /* report -1 min_distance stdout below... */
      min_reference = 32;          /* report 'No match' to stdout below... */
    } else {
      if (min_unknown % 32 == min_reference)
	hit++;
      else {
	miss++;
      }
    }

    /* report results of this unknown-reference comparison */
    sprintf(hit_miss_no_match, "%d/%d/%d", hit, miss, no_match);

    printf(":%3d %9s %10s %6.2f%% %6.3fs\n",
	   min_distance,
	   county[min_reference],
	   hit_miss_no_match,
	   100.0 * hit / (hit + miss + no_match),
	   tock(sub_time));
  } /* outer loop over the unknowns complete */

  /* we're done - report parms and stats for this run */
  printf("window %2d r%3d l%d a%d hit %3d (%6.2f%%) miss %3d (%6.2f%%) no_match %3d (%6.2f%%)\n",
	 window, ref_lo, l_flag, a_flag,
	 hit,      100.0 * hit      / (hit + miss + no_match),
	 miss,     100.0 * miss     / (hit + miss + no_match),
	 no_match, 100.0 * no_match / (hit + miss + no_match));

  /* cleanup */
  for (u = 0; u < 32; u++)
    free(utt_data[u].data);

  fclose(fp);

  /* report elapsed time in seconds */
  printf("that took %fs\n", tock(run_time));

  return 0;
}
