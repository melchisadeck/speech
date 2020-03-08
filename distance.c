#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "utterance.h"
#include "distance.h"

#define TRUE  1
#define FALSE 0

unsigned int matrix[64][64];

void compute_matrix(utt_data, r, u, window)
int r, u, window;
u_data *utt_data;
{
  int i, j, k, d;
  int ref_length   = utt_data[r].length;
  int unk_length   = utt_data[u].length;
  unsigned char *p = utt_data[r].data;
  unsigned char *q = utt_data[u].data;

  for (i = 0; i < ref_length; i++) {
    for (j = 0; j < unk_length; j++) {

      matrix[i][j] = 0;

      for (k = 0; k < 8; k++)
	matrix[i][j] += abs(p[i * 8 + k] - q[j * 8 + k]);
    }
  }

  for (i = 1; i < ref_length; i++)
    matrix[i][0] += matrix[i-1][0];

  for (j = 1; j < unk_length; j++)
    matrix[0][j] += matrix[0][j-1];

  for (i = 1; i < ref_length; i++)
    for (j = 1; j < unk_length; j++)
      matrix[i][j] += min3(matrix[ i-1 ][ j   ],
			   matrix[ i-1 ][ j-1 ],
			   matrix[ i   ][ j-1 ]);
}

int min_path(utt_data, r, u, window)
int r, u, window;
u_data *utt_data;
{
  int i, j, k;
  int n, ne, e;
  int sum_distance;
  int ref_length = utt_data[r].length;
  int unk_length = utt_data[u].length;

  int out_of_window = FALSE;
  double slope      = (double) ref_length / (double) unk_length;

  i = j = 0;

  do {
    n  = matrix[ i+1 ][ j   ];
    ne = matrix[ i+1 ][ j+1 ];
    e  = matrix[ i   ][ j+1 ];

    k = min3(n, ne, e);

    if (k == ne) {
      i++;
      j++;
    } else if (k == n)
      i++;
    else
      j++;

    out_of_window = (fabs(slope * (double) j - (double) i) > (double) window);

  } while (! out_of_window && i < ref_length-1 && j < unk_length-1);

  if (out_of_window)
    sum_distance = -1;
  else
    sum_distance = (double) matrix[i][j] / (double) (ref_length + unk_length);

  return sum_distance;
}

void print_matrix(utt_data, r, u)
int r, u;
u_data *utt_data;
{
  int i, j;

  printf("\n");

  for (i = 0; i < utt_data[r].length; i++) {
    for (j = 0; j < utt_data[u].length; j++)
      printf(" %u", matrix[i][j]);

    printf("\n");
  }

  printf("\n");
}

int min3(x, y, z)
int x, y, z;
{
  int min;

  if (x <= y)
    if (x <= z)
      min = x;
    else
      min = z;
  else if (x <= z)
    min = y;
  else
    if (y <= z)
      min = y;
    else
      min = z;

  return min;
}
