/* Conversion lookup tables from conventional alpha to Berlekamp's
 * dual-basis representation. Used in the CCSDS version only.
 * conventional_to_dual[] -- convert conventional to dual basis
 * dual_to_conventional[] -- convert dual basis to conventional

 * Note: the actual RS encoder/decoder works with the conventional basis.
 * So data is converted from dual to conventional basis before either
 * encoding or decoding and then converted back.
 *
 * Copyright 2002 Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#include <stdio.h>
#include <stdlib.h>

#define DTYPE unsigned char
DTYPE conventional_to_dual[256], dual_to_conventional[256];

static DTYPE tal[] = {0x8d, 0xef, 0xec, 0x86, 0xfa, 0x99, 0xaf, 0x7b};

/* Generate conversion lookup tables between conventional alpha representation
 * (@**7, @**6, ...@**0)
 *  and Berlekamp's dual basis representation
 * (l0, l1, ...l7)
 */
int main()
{
  int i, j, k;

  for (i = 0; i < 256; i++)
  { /* For each value of input */
    conventional_to_dual[i] = 0;
    for (j = 0; j < 8; j++) /* for each column of matrix */
      for (k = 0; k < 8; k++)
      { /* for each row of matrix */
        if (i & (1 << k))
          conventional_to_dual[i] ^= tal[7 - k] & (1 << j);
      }
    dual_to_conventional[conventional_to_dual[i]] = i;
  }
  printf("unsigned char conventional_to_dual[] = {");
  for (i = 0; i < 256; i++)
  {
    if ((i % 16) == 0)
      printf("\n");
    printf("0x%02x, ", conventional_to_dual[i]);
  }
  printf("\n};\n\nunsigned char dual_to_conventional[] = {");
  for (i = 0; i < 256; i++)
  {
    if ((i % 16) == 0)
      printf("\n");
    printf("0x%02x, ", dual_to_conventional[i]);
  }
  printf("\n};\n");
  exit(0);
}