#ifndef UTTERANCE_H
#define UTTERANCE_H

#define NUM_UTTERANCE 608

typedef struct {
  unsigned char length;
  unsigned char max;
  long          fpos;
  unsigned char *data;
} u_data;

extern char *county[];

void init_utterance();
unsigned char *read_utterance();
void print_utterance();
void log_scale();
void amp_normalise();

#endif /* UTTERANCE_H */
